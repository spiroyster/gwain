#ifndef GWAIN_ARTIST_FILTER_HPP
#define GWAIN_ARTIST_FILTER_HPP

#include "..\core\image.hpp"

#include <functional>

namespace gwain
{
	namespace artist
	{
		typedef std::function<rgba32(i x, i y, const rgba32& pixel)> FilterCallback;

		static rgba32 Invert(const rgba32& imagePixel)
		{
			return rgba32(static_cast<unsigned char>(255 - imagePixel.r_), static_cast<unsigned char>(255 - imagePixel.g_), static_cast<unsigned char>(255 - imagePixel.b_), static_cast<unsigned char>(255 - imagePixel.a_));
		}

		static rgba32 InvertNoAlpha(const rgba32& imagePixel)
		{
			return rgba32(static_cast<unsigned char>(255 - imagePixel.r_), static_cast<unsigned char>(255 - imagePixel.g_), static_cast<unsigned char>(255 - imagePixel.b_), imagePixel.a_);
		}

		static void filter(image& canvas, i x, i y, i width, i height, FilterCallback callback)
		{
			for (int yy = y; yy < height; ++yy)
				for (int xx = x; xx < width; ++xx)
				{
					if (yy >= 0 && yy < height && xx >= 0 && xx < width)
					{
						canvas.pixel(xx, yy) = callback(xx, yy, canvas.pixel(xx, yy));
					}
				}
		}

		static void mix(image& canvas, const rgb24& colour)
		{
			rgb96 mixer = to_rgb96(colour);
			filter(canvas, 0, 0, canvas.image_width(), canvas.image_height(),
				[&mixer](i x, i y, const rgba32& pix)
				{
					rgba128 col = to_rgba128(pix);
					col.r_ *= mixer.r_;
					col.g_ *= mixer.g_;
					col.b_ *= mixer.b_;
					return to_rgba32(col);
				});
		}


	}
}

#endif // GWAIN_ARTIST_FILTER_HPP