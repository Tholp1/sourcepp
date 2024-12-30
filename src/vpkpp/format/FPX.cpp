#include <vpkpp/format/FPX.h>

#include <filesystem>
#include <FileStream.h>
#include <sourcepp/String.h>

using namespace sourcepp;
using namespace vpkpp;

std::unique_ptr<PackFile> FPX::create(const std::string& path) {
	{
		FileStream stream{path, FileStream::OPT_TRUNCATE | FileStream::OPT_CREATE_IF_NONEXISTENT};

		Header1 header1{};
		header1.signature = FPX_SIGNATURE;
		header1.version = 10;
		header1.treeSize = 1;
		stream.write(header1);

		stream.write('\0');
	}
	return FPX::open(path);
}

std::unique_ptr<PackFile> FPX::open(const std::string& path, const EntryCallback& callback) {
	std::unique_ptr<PackFile> fpx;

	// Try loading the directory FPX first if this is a numbered archive and the dir exists
	if (path.length() >= 8) {
		auto dirPath = path.substr(0, path.length() - 8) + "_fdr.fpx";
		auto pathEnd = path.substr(path.length() - 8, path.length());
		if (string::matches(pathEnd, "_%d%d%d.fpx") && std::filesystem::exists(dirPath)) {
			fpx = FPX::openInternal(dirPath, callback);
			if (fpx) {
				return fpx;
			}
		}
	}

	return FPX::openInternal(path, callback);
}

std::unique_ptr<PackFile> FPX::openInternal(const std::string& path, const EntryCallback& callback) {
	if (!std::filesystem::exists(path)) {
		// File does not exist
		return nullptr;
	}

	auto* fpx = new FPX{path};
	auto packFile = std::unique_ptr<PackFile>(fpx);

	FileStream reader{fpx->fullFilePath};
	reader.seek_in(0);
	reader.read(fpx->header1);
	if (fpx->header1.signature != FPX_SIGNATURE) {
		// File is not an FPX
		return nullptr;
	}
	if (fpx->header1.version != 10) {
		// Only support v10 FPX files
		return nullptr;
	}

	// Extensions
	while (true) {
		std::string extension;
		reader.read(extension);
		if (extension.empty())
			break;

		// Directories
		while (true) {
			std::string directory;
			reader.read(directory);
			if (directory.empty())
				break;

			std::string fullDir;
			if (directory == " ") {
				fullDir = "";
			} else {
				fullDir = directory;
			}

			// Files
			while (true) {
				std::string entryName;
				reader.read(entryName);
				if (entryName.empty())
					break;

				Entry entry = createNewEntry();

				std::string entryPath;
				if (extension == " ") {
					entryPath = fullDir.empty() ? "" : fullDir + '/';
					entryPath += entryName;
				} else {
					entryPath = fullDir.empty() ? "" : fullDir + '/';
					entryPath += entryName + '.';
					entryPath += extension;
				}
				entryPath = fpx->cleanEntryPath(entryPath);

				reader.read(entry.crc32);
				auto preloadedDataSize = reader.read<uint16_t>();
				entry.archiveIndex = reader.read<uint16_t>();
				entry.offset = reader.read<uint32_t>();
				entry.length = reader.read<uint32_t>();

				if (reader.read<uint16_t>() != VPK_ENTRY_TERM) {
					// Invalid terminator!
					return nullptr;
				}

				if (preloadedDataSize > 0) {
					entry.extraData = reader.read_bytes(preloadedDataSize);
					entry.length += preloadedDataSize;
				}

				if (entry.archiveIndex != VPK_DIR_INDEX && entry.archiveIndex > fpx->numArchives) {
					fpx->numArchives = entry.archiveIndex;
				}

				fpx->entries.emplace(entryPath, entry);

				if (callback) {
					callback(entryPath, entry);
				}
			}
		}
	}

	// If there are no archives, -1 will be incremented to 0
	fpx->numArchives++;

	return packFile;
}
