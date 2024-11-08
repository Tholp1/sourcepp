#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <span>
#include <vector>

#ifdef SOURCEPP_BUILD_WITH_TBB
#include <execution>
#endif

#include <BufferStream.h>
#include <sourcepp/Templates.h>

#include "ImageFormats.h"

namespace vtfpp {

namespace ImagePixel {

#define VTFPP_CHECK_SIZE(format) \
	static_assert(sizeof(format) == ImageFormatDetails::bpp(ImageFormat::format) / 8)

struct RGBA8888 {
	static constexpr auto FORMAT = ImageFormat::RGBA8888;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};
VTFPP_CHECK_SIZE(RGBA8888);

struct ABGR8888 {
	static constexpr auto FORMAT = ImageFormat::ABGR8888;
	uint8_t a;
	uint8_t b;
	uint8_t g;
	uint8_t r;
};
VTFPP_CHECK_SIZE(ABGR8888);

struct RGB888 {
	static constexpr auto FORMAT = ImageFormat::RGB888;
	uint8_t r;
	uint8_t g;
	uint8_t b;
};
VTFPP_CHECK_SIZE(RGB888);

struct RGB888_BLUESCREEN : RGB888 {
	static constexpr auto FORMAT = ImageFormat::RGB888_BLUESCREEN;
};
VTFPP_CHECK_SIZE(RGB888_BLUESCREEN);

struct BGR888 {
	static constexpr auto FORMAT = ImageFormat::BGR888;
	uint8_t b;
	uint8_t g;
	uint8_t r;
};
VTFPP_CHECK_SIZE(BGR888);

struct BGR888_BLUESCREEN : BGR888 {
	static constexpr auto FORMAT = ImageFormat::BGR888_BLUESCREEN;
};
VTFPP_CHECK_SIZE(BGR888_BLUESCREEN);

struct RGB565 {
	static constexpr auto FORMAT = ImageFormat::RGB565;
	uint16_t r : 5;
	uint16_t g : 6;
	uint16_t b : 5;
};
VTFPP_CHECK_SIZE(RGB565);

struct I8 {
	static constexpr auto FORMAT = ImageFormat::I8;
	uint8_t i;
};
VTFPP_CHECK_SIZE(I8);

struct IA88 {
	static constexpr auto FORMAT = ImageFormat::IA88;
	uint8_t i;
	uint8_t a;
};
VTFPP_CHECK_SIZE(IA88);

struct P8 {
	static constexpr auto FORMAT = ImageFormat::P8;
	uint8_t p;
};
VTFPP_CHECK_SIZE(P8);

struct A8 {
	static constexpr auto FORMAT = ImageFormat::A8;
	uint8_t a;
};
VTFPP_CHECK_SIZE(A8);

struct ARGB8888 {
	static constexpr auto FORMAT = ImageFormat::ARGB8888;
	uint8_t a;
	uint8_t r;
	uint8_t g;
	uint8_t b;
};
VTFPP_CHECK_SIZE(ARGB8888);

struct BGRA8888 {
	static constexpr auto FORMAT = ImageFormat::BGRA8888;
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
};
VTFPP_CHECK_SIZE(BGRA8888);

struct BGRX8888 {
	static constexpr auto FORMAT = ImageFormat::BGRX8888;
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t x;
};
VTFPP_CHECK_SIZE(BGRX8888);

struct BGR565 {
	static constexpr auto FORMAT = ImageFormat::BGR565;
	uint16_t b : 5;
	uint16_t g : 6;
	uint16_t r : 5;
};
VTFPP_CHECK_SIZE(BGR565);

struct BGRX5551 {
	static constexpr auto FORMAT = ImageFormat::BGRX5551;
	uint16_t b : 5;
	uint16_t g : 5;
	uint16_t r : 5;
	uint16_t x : 1;
};
VTFPP_CHECK_SIZE(BGRX5551);

struct BGRA4444 {
	static constexpr auto FORMAT = ImageFormat::BGRA4444;
	uint16_t b : 4;
	uint16_t g : 4;
	uint16_t r : 4;
	uint16_t a : 4;
};
VTFPP_CHECK_SIZE(BGRA4444);

struct BGRA5551 {
	static constexpr auto FORMAT = ImageFormat::BGRA5551;
	uint16_t b : 5;
	uint16_t g : 5;
	uint16_t r : 5;
	uint16_t a : 1;
};
VTFPP_CHECK_SIZE(BGRA5551);

struct UV88 {
	static constexpr auto FORMAT = ImageFormat::UV88;
	uint8_t u;
	uint8_t v;
};
VTFPP_CHECK_SIZE(UV88);

struct UVWQ8888 {
	static constexpr auto FORMAT = ImageFormat::UVWQ8888;
	uint8_t u;
	uint8_t v;
	uint8_t w;
	uint8_t q;
};
VTFPP_CHECK_SIZE(UVWQ8888);

struct RGBA16161616F {
	static constexpr auto FORMAT = ImageFormat::RGBA16161616F;
	half r;
	half g;
	half b;
	half a;
};
VTFPP_CHECK_SIZE(RGBA16161616F);

struct RGBA16161616 {
	static constexpr auto FORMAT = ImageFormat::RGBA16161616;
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t a;
};
VTFPP_CHECK_SIZE(RGBA16161616);

struct UVLX8888 {
	static constexpr auto FORMAT = ImageFormat::UVLX8888;
	uint8_t u;
	uint8_t v;
	uint8_t l;
	uint8_t x;
};
VTFPP_CHECK_SIZE(UVLX8888);

struct R32F {
	static constexpr auto FORMAT = ImageFormat::R32F;
	float r;
};
VTFPP_CHECK_SIZE(R32F);

struct RGB323232F {
	static constexpr auto FORMAT = ImageFormat::R32F;
	float r;
	float g;
	float b;
};
VTFPP_CHECK_SIZE(RGB323232F);

struct RGBA32323232F {
	static constexpr auto FORMAT = ImageFormat::RGBA32323232F;
	float r;
	float g;
	float b;
	float a;
};
VTFPP_CHECK_SIZE(RGBA32323232F);

struct RG1616F {
	static constexpr auto FORMAT = ImageFormat::RG1616F;
	half r;
	half g;
};
VTFPP_CHECK_SIZE(RG1616F);

struct RG3232F {
	static constexpr auto FORMAT = ImageFormat::RG3232F;
	float r;
	float g;
};
VTFPP_CHECK_SIZE(RG3232F);

struct RGBX8888 {
	static constexpr auto FORMAT = ImageFormat::RGBX8888;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t x;
};
VTFPP_CHECK_SIZE(RGBX8888);

struct RGBA1010102 {
	static constexpr auto FORMAT = ImageFormat::RGBA1010102;
	uint32_t r : 10;
	uint32_t g : 10;
	uint32_t b : 10;
	uint32_t a : 2;
};
VTFPP_CHECK_SIZE(RGBA1010102);

struct BGRA1010102 {
	static constexpr auto FORMAT = ImageFormat::BGRA1010102;
	uint32_t b : 10;
	uint32_t g : 10;
	uint32_t r : 10;
	uint32_t a : 2;
};
VTFPP_CHECK_SIZE(BGRA1010102);

struct R16F {
	static constexpr auto FORMAT = ImageFormat::R16F;
	half r;
};
VTFPP_CHECK_SIZE(R16F);

struct R8 {
	static constexpr auto FORMAT = ImageFormat::R8;
	uint8_t r;
};
VTFPP_CHECK_SIZE(R8);

#undef VTFPP_CHECK_SIZE

template<typename T>
concept PixelType =
		std::same_as<T, RGBA8888> ||
		std::same_as<T, ABGR8888> ||
		std::same_as<T, RGB888> ||
		std::same_as<T, BGR888> ||
		std::same_as<T, RGB565> ||
		std::same_as<T, I8> ||
		std::same_as<T, IA88> ||
		std::same_as<T, P8> ||
		std::same_as<T, A8> ||
		std::same_as<T, RGB888_BLUESCREEN> ||
		std::same_as<T, BGR888_BLUESCREEN> ||
		std::same_as<T, ARGB8888> ||
		std::same_as<T, BGRA8888> ||
		std::same_as<T, BGRX8888> ||
		std::same_as<T, BGR565> ||
		std::same_as<T, BGRX5551> ||
		std::same_as<T, BGRA4444> ||
		std::same_as<T, BGRA5551> ||
		std::same_as<T, UV88> ||
		std::same_as<T, UVWQ8888> ||
		std::same_as<T, RGBA16161616F> ||
		std::same_as<T, RGBA16161616> ||
		std::same_as<T, UVLX8888> ||
		std::same_as<T, R32F> ||
		std::same_as<T, RGB323232F> ||
		std::same_as<T, RGBA32323232F> ||
		std::same_as<T, RG1616F> ||
		std::same_as<T, RG3232F> ||
		std::same_as<T, RGBX8888> ||
		std::same_as<T, RGBA1010102> ||
		std::same_as<T, BGRA1010102> ||
		std::same_as<T, R16F> ||
		std::same_as<T, R8>;

} // namespace ImagePixel

namespace ImageConversion {

/// Converts an image from one format to another.
[[nodiscard]] std::vector<std::byte> convertImageDataToFormat(std::span<const std::byte> imageData, ImageFormat oldFormat, ImageFormat newFormat, uint16_t width, uint16_t height);

/// Converts several images from one format to another.
[[nodiscard]] std::vector<std::byte> convertSeveralImageDataToFormat(std::span<const std::byte> imageData, ImageFormat oldFormat, ImageFormat newFormat, uint8_t mipCount, uint16_t frameCount, uint16_t faceCount, uint16_t width, uint16_t height, uint16_t sliceCount);

enum class FileFormat {
	DEFAULT,
	PNG,
	JPEG,
	BMP,
	TGA,
	HDR,
	EXR,
};

/// PNG for integer formats, EXR for floating point formats
[[nodiscard]] FileFormat getDefaultFileFormatForImageFormat(ImageFormat format);

/// Converts image data to a PNG or EXR file. EXR format will be used for floating-point image formats.
[[nodiscard]] std::vector<std::byte> convertImageDataToFile(std::span<const std::byte> imageData, ImageFormat format, uint16_t width, uint16_t height, FileFormat fileFormat = FileFormat::DEFAULT);

[[nodiscard]] std::vector<std::byte> convertFileToImageData(std::span<const std::byte> fileData, ImageFormat& format, int& width, int& height, int& frameCount);

enum class ResizeEdge {
	// Matches stbir_edge
	CLAMP = 0,
	REFLECT,
	WRAP,
	ZERO,
};

enum class ResizeFilter {
	// Matches stbir_filter
	DEFAULT = 0,
	BOX,
	BILINEAR,
	CUBIC_BSPLINE,
	CATMULLROM,
	MITCHELL,
	// Disabled for now, see https://github.com/nothings/stb/issues/1678
	//POINT_SAMPLE,
};

enum class ResizeMethod {
	NONE,
	POWER_OF_TWO_BIGGER,
	POWER_OF_TWO_SMALLER,
	POWER_OF_TWO_NEAREST,
};

/// Get the new image size given a resize method
[[nodiscard]] uint16_t getResizedDim(uint16_t n, ResizeMethod method);

/// Set the new image dimensions given a resize method
void setResizedDims(uint16_t& width, ResizeMethod widthResize, uint16_t& height, ResizeMethod heightResize);

/// Resize given image data to the new dimensions
[[nodiscard]] std::vector<std::byte> resizeImageData(std::span<const std::byte> imageData, ImageFormat format, uint16_t width, uint16_t newWidth, uint16_t height, uint16_t newHeight, bool srgb, ResizeFilter filter, ResizeEdge edge = ResizeEdge::CLAMP);

/// Resize given image data to the new dimensions, where the new width and height are governed by the resize methods
[[nodiscard]] std::vector<std::byte> resizeImageDataStrict(std::span<const std::byte> imageData, ImageFormat format, uint16_t width, uint16_t newWidth, uint16_t& widthOut, ResizeMethod widthResize, uint16_t height, uint16_t newHeight, uint16_t& heightOut, ResizeMethod heightResize, bool srgb, ResizeFilter filter, ResizeEdge edge = ResizeEdge::CLAMP);

/// Extracts a single channel from the given image data.
/// May have unexpected behavior if called on formats that use bitfields like BGRA5551!
/// Data is packed according to pixel channel C++ type size
/// (e.g. in the case of BGRA5551's green channel, it'll be 2 bytes per green value despite only 5 bits being used in the original data)
template<ImagePixel::PixelType P>
[[nodiscard]] std::vector<std::byte> extractChannelFromImageData(std::span<const std::byte> imageData, auto P::*channel) {
	using C = sourcepp::member_type_t<decltype(channel)>;
	if (imageData.empty() || imageData.size() % sizeof(P) != 0) {
		return {};
	}

	std::span pixels{reinterpret_cast<const P*>(imageData.data()), imageData.size() / sizeof(P)};

	std::vector<std::byte> out(imageData.size() / sizeof(P) * sizeof(C));
	std::transform(
#ifdef SOURCEPP_BUILD_WITH_TBB
			std::execution::par_unseq,
#endif
			pixels.begin(), pixels.end(), out.begin(), [channel](P pixel) -> C {
		return pixel.*channel;
	});

	return out;
}

/// Applies a single channel to the given image data.
/// May have unexpected behavior if called on formats that use bitfields like BGRA5551!
/// Data is packed according to pixel channel C++ type size
/// (e.g. in the case of BGRA5551's green channel, it'll be 2 bytes per green value despite only 5 bits being used in the original data)
template<ImagePixel::PixelType P>
bool applyChannelToImageData(std::span<std::byte> imageData, std::span<const std::byte> channelData, auto P::*channel) {
	using C = sourcepp::member_type_t<decltype(channel)>;
	if (imageData.empty() || imageData.size() % sizeof(P) != 0 || channelData.empty() || channelData.size() % sizeof(C) != 0 || imageData.size() / sizeof(P) != channelData.size() / sizeof(C)) {
		return false;
	}

	std::span pixels{reinterpret_cast<P*>(imageData.data()), imageData.size() / sizeof(P)};
	std::span values{reinterpret_cast<C*>(channelData.data()), channelData.size() / sizeof(C)};

	for (int i = 0; i < pixels.size(); i++) {
		pixels[i].*channel = values[i];
	}
	return true;
}

} // namespace ImageConversion

} // namespace vtfpp
