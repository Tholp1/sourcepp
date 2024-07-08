#include <sourcepp/parser/Text.h>

#include <algorithm>
#include <cctype>

#include <BufferStream.h>

using namespace sourcepp;

namespace sourcepp::parser::text {

const std::unordered_map<char, char> DEFAULT_ESCAPE_SEQUENCES = {
		{'\'', '\''},
		{'\"', '\"'},
		{'?',   '?'},
		{'\\', '\\'},
		{'a',  '\a'},
		{'b',  '\b'},
		{'f',  '\f'},
		{'n',  '\n'},
		{'r',  '\r'},
		{'t',  '\t'},
		{'v',  '\v'},
};

} // namespace parser::text

bool parser::text::isNewLine(char c) {
	return c == '\n' || c == '\r';
}

bool parser::text::isWhitespace(char c) {
	return c == ' ' || c == '\a' || c == '\f' || c == '\t' || c == '\v' || isNewLine(c);
}

bool parser::text::isNumber(char c) {
	return std::isdigit(c);
}

void parser::text::eatSingleLineComment(BufferStream& stream) {
	while (!isNewLine(stream.read<char>())) {}
}

void parser::text::eatMultiLineComment(BufferStream& stream, std::string_view multiLineCommentEnd) {
	while (!std::ranges::equal(stream.read_span<char>(multiLineCommentEnd.length()), multiLineCommentEnd)) {
		stream.seek(-static_cast<int64_t>(multiLineCommentEnd.length() - 1), std::ios::cur);
	}
}

void parser::text::eatWhitespaceAndSingleLineComments(BufferStream& stream, std::string_view singleLineCommentStart) {
	return eatWhitespaceAndComments(stream, singleLineCommentStart, "");
}

void parser::text::eatWhitespaceAndMultiLineComments(BufferStream& stream, std::string_view multiLineCommentStart) {
	return eatWhitespaceAndComments(stream, "", multiLineCommentStart);
}

// NOLINTNEXTLINE(*-no-recursion)
void parser::text::eatWhitespaceAndComments(BufferStream& stream, std::string_view singleLineCommentStart, std::string_view multiLineCommentStart) {
	while (isWhitespace(stream.read<char>())) {}
	stream.seek(-1, std::ios::cur);

	if (!singleLineCommentStart.empty()) {
		if (std::ranges::equal(stream.read_span<char>(singleLineCommentStart.length()), singleLineCommentStart)) {
			text::eatSingleLineComment(stream);
			eatWhitespaceAndComments(stream, singleLineCommentStart, multiLineCommentStart);
			return;
		} else {
			stream.seek(-static_cast<int64_t>(singleLineCommentStart.length()), std::ios::cur);
		}
	}

	if (!multiLineCommentStart.empty()) {
		if (std::ranges::equal(stream.read_span<char>(multiLineCommentStart.length()), multiLineCommentStart)) {
			text::eatMultiLineComment(stream);
			eatWhitespaceAndComments(stream, singleLineCommentStart, multiLineCommentStart);
			return;
		} else {
			stream.seek(-static_cast<int64_t>(multiLineCommentStart.length()), std::ios::cur);
		}
	}
}

std::string_view parser::text::readStringToBuffer(BufferStream& stream, BufferStream& backing, std::string_view start, std::string_view end, const std::unordered_map<char, char>& escapeSequences) {
	auto startSpan = backing.tell();

	bool stopAtWhitespace = true;
	char c = stream.read<char>();
	if (start.find(c) != std::string_view::npos) {
		stopAtWhitespace = false;
	} else {
		backing << c;
	}

	for (c = stream.read<char>(); (stopAtWhitespace && !parser::text::isWhitespace(c)) || (!stopAtWhitespace && end.find(c) == std::string_view::npos); c = stream.read<char>()) {
		if (!escapeSequences.empty() && c == '\\') {
			auto n = stream.read<char>();
			if (stopAtWhitespace && parser::text::isWhitespace(n)) {
				break;
			}
			if (escapeSequences.contains(n)) {
				backing << escapeSequences.at(n);
			} else if (!stopAtWhitespace && end.find(n) != std::string_view::npos) {
				break;
			} else {
				backing << c << n;
			}
		} else {
			backing << c;
		}
	}

	backing << '\0';
	return {reinterpret_cast<const char*>(backing.data()) + startSpan, backing.tell() - 1 - startSpan};
}
