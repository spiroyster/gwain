#ifndef GWAIN_ARTIST_BORDER_HPP
#define GWAIN_ARTIST_BORDER_HPP

#include "paint.hpp"

namespace gwain
{
	namespace artist
	{
		static image border(ui width, ui height, const rgba32& fillColour, const rgba32& borderColour, ui borderThickness)
		{
			image result(borderColour, width, height);
			image infill(fillColour, width - (2 * borderThickness), height - (2 * borderThickness));
			paint(result, (result.image_width() - infill.image_width()) / 2, (result.image_height() - infill.image_height()) / 2, infill, 0, 0, infill.image_width(), infill.image_height(), Overwrite);
			return result;
		}

		static float vector_length(float x, float y)
		{
			return sqrt((x * x) + (y * y));
		}

		void fillet(image& canvas, i bottomLeftRadius, i bottomRightRadius, i topRightRadius, i topLeftRadius)
		{
			// Bresenham circle algorithm...

			// bottom left...
			int centreX = bottomLeftRadius - 1;
			int centreY = bottomLeftRadius - 1;
			for (int y = 0; y < centreY; ++y)
				for (int x = 0; x < centreX; ++x)
				{
					// Check distance from centre
					if (vector_length(static_cast<float>(x) - static_cast<float>(centreX), static_cast<float>(y) - static_cast<float>(centreY)) < static_cast<float>(bottomLeftRadius))
					{
						const rgba32& pix = canvas.pixel(x, y);
						canvas.pixel(x, y) = rgba32(pix.r_, pix.g_, pix.b_, 0);
					}
				}

			// Top left..
			centreX = topLeftRadius - 1;
			centreY = canvas.image_height() - topLeftRadius;
			for (int y = centreY; y < static_cast<int>(canvas.image_height()); ++y)
				for (int x = 0; x < centreX; ++x)
				{
					// Check distance from centre
					if (vector_length(static_cast<float>(x) - static_cast<float>(centreX), static_cast<float>(y) - static_cast<float>(centreY)) < static_cast<float>(topLeftRadius))
					{
						const rgba32& pix = canvas.pixel(x, y);
						canvas.pixel(x, y) = rgba32(pix.r_, pix.g_, pix.b_, 0);
					}
				}

			// top right...
			centreX = canvas.image_width() - topRightRadius;
			centreY = canvas.image_width() - topRightRadius;
			for (int y = centreY; y < static_cast<int>(canvas.image_height()); ++y)
				for (int x = centreX; x < static_cast<int>(canvas.image_height()); ++x)
				{
					// Check distance from centre
					if (vector_length(static_cast<float>(x) - static_cast<float>(centreX), static_cast<float>(y) - static_cast<float>(centreY)) < static_cast<float>(topRightRadius))
					{
						const rgba32& pix = canvas.pixel(x, y);
						canvas.pixel(x, y) = rgba32(pix.r_, pix.g_, pix.b_, 0);
					}
				}

			// bottom right...
			centreX = canvas.image_width() - bottomRightRadius;
			centreY = bottomRightRadius - 1;
			for (int y = 0; y < centreY; ++y)
				for (int x = centreX; x < static_cast<int>(canvas.image_width()); ++x)
				{
					// Check distance from centre
					if (vector_length(static_cast<float>(x) - static_cast<float>(centreX), static_cast<float>(y) - static_cast<float>(centreY)) < static_cast<float>(bottomRightRadius))
					{
						const rgba32& pix = canvas.pixel(x, y);
						canvas.pixel(x, y) = rgba32(pix.r_, pix.g_, pix.b_, 0);
					}
				}

		}

		image filleted_border(ui width, ui height, ui borderThickness, const rgba32& borderColour, const rgba32& fillColour,
			int bottomLeftRadius, int bottomRightRadius, int topRightRadius, int topLeftRadius)
		{
			image background(borderColour, width, height);
			image foreground(fillColour, width - (2 * borderThickness), height - (2 * borderThickness));
			fillet(background, bottomLeftRadius, bottomLeftRadius, bottomLeftRadius, bottomLeftRadius);
			fillet(foreground, bottomLeftRadius, bottomLeftRadius, bottomLeftRadius, bottomLeftRadius);
			return centred(background, foreground);
		}
	}
}

#endif // GWAIN_ARTIST_BORDER_HPP