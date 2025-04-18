#ifndef GWAIN_ARTIST_PAINT_HPP
#define GWAIN_ARTIST_PAINT_HPP

#include "..\core\image.hpp"

#include <functional>

namespace gwain
{
	namespace artist
	{
		typedef std::function<rgba32(const rgba32& image, const rgba32& brush)> PaintCallback;

		static rgba32 PaintersAlgorithm(const rgba32& image, const rgba32& brush)
		{
			if (brush.a_ == 255)
				return brush;

			// Blend them...
			rgba128 brush128 = to_rgba128(brush);
			rgba128 image128 = to_rgba128(image);

			image128 = rgba128(
				brush128.r_ * brush128.a_ + ((1.0f - brush128.a_) * image128.r_ * image128.a_),
				brush128.g_ * brush128.a_ + ((1.0f - brush128.a_) * image128.g_ * image128.a_),
				brush128.b_ * brush128.a_ + ((1.0f - brush128.a_) * image128.b_ * image128.a_),
				brush128.a_ + ((1.0f - brush128.a_) * image128.a_)
			);

			return to_rgba32(image128);
		}

		static rgba32 Overwrite(const rgba32& imagePixel, const rgba32& brushPixel)
		{
			return brushPixel;
		}

		static rgba32 OverwriteNoAlpha(const rgba32& imagePixel, const rgba32& brushPixel)
		{
			return rgba32(brushPixel.r_, brushPixel.g_, brushPixel.b_, imagePixel.a_);
		}

		static void paint(image& img, i x, i y, const image& brush, i brush_x, i brush_y, i brush_width, i brush_height, PaintCallback callback)
		{
			if (brush_x < 0 || brush_y < 0)
				throw exception(error::paintBrushInvalid);

			for (int yy = 0; yy < brush_height; ++yy)
				for (int xx = 0; xx < brush_width; ++xx)
				{
					int imgX = x + xx;
					int imgY = y + yy;
					int brsX = xx + brush_x;
					int brsY = yy + brush_y;

					if (imgX >= 0 && imgX < static_cast<int>(img.image_width()) && imgY >= 0 && imgY < static_cast<int>(img.image_height()) &&
						brsX >= 0 && brsX < static_cast<int>(brush.image_width()) && brsY >= 0 && brsY < static_cast<int>(brush.image_height()))
					{
						rgba32& pixel = img.pixel(imgX, imgY);
						pixel = callback(pixel, brush.pixel(brsX, brsY));
					}
				}
		}

		static void pad(image& img, i padLeft, i padRight, i padTop, i padBottom, const rgba32& colour)
		{
			int newImageWidth = img.image_width() + padLeft + padRight;
			int newImageHeight = img.image_height() + padTop + padBottom;

			auto canvas = image(colour, newImageWidth, newImageHeight);
			paint(canvas, padLeft, padBottom, img, 0, 0, img.image_width(), img.image_height(), PaintersAlgorithm);
			img = canvas;
		}

		static void plot(image& img, i x0, i y0, i x1, i y1, const rgba32& colour)
		{
			int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
			int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
			int err = dx + dy, e2; /* error value e_xy */

			for (;;)
			{
				if (x0 >= 0 && x0 < static_cast<int>(img.image_width()) && (y0 >= 0 && y0 < static_cast<int>(img.image_height())))
					img.pixel(x0, y0) = colour;

				if (x0 == x1 && y0 == y1)
					break;
				e2 = 2 * err;

				if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
				if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
			}
		}

		static image centred(const image& canvas, const image& brush)
		{
			image result = canvas;
			paint(result, 
				(static_cast<int>(result.image_width()) - static_cast<int>(brush.image_width())) / 2, 
				(static_cast<int>(result.image_height()) - static_cast<int>(brush.image_height())) / 2, 
				brush, 0, 0, brush.image_width(), brush.image_height(), PaintersAlgorithm);
			return result;
		}
	}
}

#endif // GWAIN_ARTIST_PAINT_HPP