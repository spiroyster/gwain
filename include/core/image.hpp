#ifndef GWAIN_CORE_IMAGE_HPP
#define GWAIN_CORE_IMAGE_HPP

#include "types.hpp"
#include "error.hpp"

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "..\stb\stb_image.h"
#pragma warning(disable : 4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\stb\stb_image_write.h"

namespace gwain
{

	struct rgba32
	{
		rgba32() : r_(0), g_(0), b_(0), a_(0) {}
		rgba32(uc r, uc g, uc b, uc a) : r_(r), g_(g), b_(b), a_(a) {}

		uc r_;
		uc g_;
		uc b_;
		uc a_;
	};

	struct rgb24
	{
		rgb24() : r_(0), g_(0), b_(0) {}
		rgb24(uc r, uc g, uc b) : r_(r), g_(g), b_(b) {}

		uc r_;
		uc g_;
		uc b_;
	};

	struct rgba128
	{
		rgba128() : r_(0), g_(0), b_(0), a_(0) {}
		rgba128(f r, f g, f b, f a) : r_(r), g_(g), b_(b), a_(a) {}

		f r_;
		f g_;
		f b_;
		f a_;
	};

	struct rgb96
	{
		rgb96() : r_(0), g_(0), b_(0) {}
		rgb96(f r, f g, f b) : r_(r), g_(g), b_(b) {}

		f r_;
		f g_;
		f b_;
	};

	// conversion between rgba32 and rgba128
	static rgba32 to_rgba32(const rgba128& col) { return rgba32(static_cast<uc>(col.r_ * 255.0f), static_cast<uc>(col.g_ * 255.0f), static_cast<uc>(col.b_ * 255.0f), static_cast<uc>(col.a_ * 255.0f)); }
	static rgba128 to_rgba128(const rgba32& col) { return rgba128(static_cast<f>(col.r_ / 255.0f), static_cast<f>(col.g_ / 255.0f), static_cast<f>(col.b_ / 255.0f), static_cast<f>(col.a_ / 255.0f)); }
	static rgb24 to_rgb24(const rgb96& col) { return rgb24(static_cast<uc>(col.r_ * 255.0f), static_cast<uc>(col.g_ * 255.0f), static_cast<uc>(col.b_ * 255.0f)); }
	static rgb96 to_rgb96(const rgb24& col) { return rgb96(static_cast<f>(col.r_ / 255.0f), static_cast<f>(col.g_ / 255.0f), static_cast<f>(col.b_ / 255.0f)); }

	class image
	{
	public:
		image()
			: image_width_(1), image_height_(1), data_({ rgba32() })
		{
		}

		image(ui width, ui height)
			: image_width_(width), image_height_(height), data_(width * height)
		{
		}

		image(ui width, ui height, const std::vector<rgba32>& data)
			: image_width_(width), image_height_(height), data_(data)
		{
		}

		image(const rgba32& colour, ui width = 1, ui height = 1)
			: image_width_(width), image_height_(height), data_(width * height, colour)
		{
		}

		image(ui width, ui height, const rgba32& colourA, const rgba32& colourB)
			:	image_width_(width), image_height_(height), data_(width* height, colourA)
		{
			ui uStride = width / 2;
			ui vStride = height / 2;

			for (i p = 0; p < pixel_count(); ++p)
			{
				bool useColourA = true;
				ui x = p % width;
				ui y = static_cast<ui>(p / width);
				if ((x / uStride) % 2)
					useColourA = !useColourA;
				if ((y / vStride) % 2)
					useColourA = !useColourA;
				data_[p] = useColourA ? colourA : colourB;
			}
		}

#ifndef GWAIN_NOSTB
		image(const char* filename)
		{
			int channels;
			if (unsigned char* data = stbi_load(filename, &image_width_, &image_height_, &channels, 4))
			{
				data_ = std::vector<rgba32>(image_width_ * image_height_ * channels);
				for (int y = 0; y < image_height_; ++y)
				{
					for (int x = 0; x < image_width_; ++x)
					{
						int source = ((y * image_width_) + x) * 4;
						int target = ((((image_height_ - 1) - y) * image_width_) + x) * 4;
						data_[target] = rgba32(*(data + (source + 0)), *(data + (source + 1)), *(data + (source + 2)), *(data + (source + 3)));
					}
				}
				stbi_image_free(data);
			}
			else
				throw exception(error::imageReadFail, std::string(filename));
		}

		void write(const char* filename)
		{
			stbi_write_png(filename, image_width_, image_height_, 4, &data_.front(), image_width_ * (pixel_stride() / sizeof(char)));
		}
#endif

		// texel access...
		rgba32& operator[](ui index) { return data_[index]; }
		const rgba32& operator[](ui index) const { return data_[index]; }

		rgba32& pixel(ui u, ui v) { return data_[(v * image_width_) + u]; }
		const rgba32& pixel(ui u, ui v) const { return data_[(v * image_width_) + u]; }

		i image_width() const { return image_width_; }
		i image_height() const { return image_height_; }
		i pixel_count() const { return image_width_ * image_height_; }
		ui pixel_stride() const { return sizeof(rgba32); }
		i pixel_channels() const { return sizeof(rgba32) / sizeof(char); }

		const std::vector<rgba32>& image_data() const { return data_; }
		const void* image_raw() const { return static_cast<const void*>(&data_.front()); }

	private:
		std::vector<rgba32> data_;
		i image_width_, image_height_;
	};

}

#endif // GWAIN_IMAGE_HPP
