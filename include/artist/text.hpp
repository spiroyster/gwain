#ifndef GWAIN_ARTIST_TEXT_HPP
#define GWAIN_ARTIST_TEXT_HPP

#include "..\core\font.hpp"
#include "transform.hpp"
#include "paint.hpp"
#include "filter.hpp"

namespace gwain
{
	namespace artist
	{
		enum text_alignment
		{
			top,
			bottom,
			centre,
			right,
			left
		};

		image text(const font& font, const std::wstring& text, int width, int height, const text_alignment& horizontalAlignment, const text_alignment& verticalAlignment)
		{
			// build the glyph map...
			struct GlyphInfo
			{
				GlyphInfo(const glyph& glyph, int startPosition)
					: glyph_(glyph), startPosition_(startPosition)
				{
				}
				const glyph& glyph_;
				int startPosition_;
			};

			std::list<GlyphInfo> glyphs;
			int currentPosition = 1;
			int maxOffset = 0;
			for (auto cItr = text.begin(); cItr != text.end(); ++cItr)
			{
				// find the glyph...
				auto glyphItr = font.map().find(*cItr);
				if (glyphItr != font.map().end())
				{
					currentPosition += glyphItr->second.xOff_;
					glyphs.push_back(GlyphInfo(glyphItr->second, currentPosition));
					currentPosition += glyphItr->second.xAdvance_ - glyphItr->second.xOff_;
				}
				else
				{
					// Otherwise we need to insert a black char?
				}
			}
			if (glyphs.empty())
				return image(width, height);

			int textWidthInPixels = glyphs.back().startPosition_ + glyphs.back().glyph_.xAdvance_;
			int textHeightInPixels = font.max_height();

			image textImage(textWidthInPixels, textHeightInPixels);
			for (auto g = glyphs.begin(); g != glyphs.end(); ++g)
				paint(textImage, g->startPosition_, font.baseline() + g->glyph_.yOff_, font.atlas()->texture_image(), g->glyph_.x_, g->glyph_.y_, g->glyph_.width_, g->glyph_.height_, Overwrite);

			if (width == 0 && height == 0)
				return textImage;

			int imageWidth = width;
			int imageHeight = height;
			// Construct our image...
			image result(imageWidth, imageHeight);

			// Calculate the text start and end position...
			int glyphOffsetX = 0;
			int glyphOffsetY = 0;

			int textImgXoffset = 0;
			int textImgYoffset = 0;

			if (horizontalAlignment == text_alignment::right)
				textImgXoffset = imageWidth - textImage.image_width();
			if (horizontalAlignment == text_alignment::centre)
				textImgXoffset = (imageWidth - textImage.image_width()) / 2;

			if (verticalAlignment == text_alignment::top)
				textImgYoffset = imageHeight - textImage.image_height();
			if (verticalAlignment == text_alignment::centre)
				textImgYoffset = (imageHeight - font.baseline() - font.max_height()) / 2;

			paint(result, textImgXoffset, textImgYoffset, textImage, 0, 0, textWidthInPixels, textHeightInPixels, PaintersAlgorithm);
			return result;
		}

		static image caption(int leftPad, const image* prefix,
			int prefixSpacing, const std::wstring& text, const rgba32& textColour, const font& font,
			int postfixSpacing, const image* postfix, int rightPad)
		{
			auto textImage = artist::text(font, text, font.max_width() * static_cast<int>(text.size()), font.max_height(), text_alignment::centre, text_alignment::centre);
			// We the need to crop the ends to tex width size..
			// Note the first and last x coords of pixels with alpha != 0...
			int xMin = textImage.image_width();
			int xMax = 0;
			for (int y = 0; y < static_cast<int>(textImage.image_height()); ++y)
				for (int x = 0; x < static_cast<int>(textImage.image_width()); ++x)
				{
					if (static_cast<int>(textImage.pixel(x, y).a_) != 0)
					{
						if (x < xMin)
							xMin = x;
						if (x > xMax)
							xMax = x;
					}
				}

			xMax += 1;

			// Crop the text so that is only the required width...
			auto textCanvas = image(rgba32(), xMax - xMin, textImage.image_height());
			paint(textCanvas, 0, 0, textImage, xMin, 0, textCanvas.image_width(), textCanvas.image_height(), Overwrite);

			filter(textCanvas, 0, 0, textCanvas.image_width(), textCanvas.image_height(), [&textColour](i x, i y, const rgba32& imagePixel)
				{
					// Set the colour ...
					return imagePixel.a_ ? rgba32(textColour.r_, textColour.g_, textColour.b_, imagePixel.a_) : rgba32();
				});


			int canvasWidth = (prefix ? prefix->image_width() : 0) + prefixSpacing + textCanvas.image_width() + postfixSpacing + (postfix ? postfix->image_width() : 0);
			int canvasHeight = textCanvas.image_height();
			if (prefix)
				canvasHeight = (std::max)(prefix->image_height(), canvasHeight);
			if (postfix)
				canvasHeight = (std::max)(postfix->image_height(), canvasHeight);

			// Create our canvas...
			auto canvas = image(rgba32(), canvasWidth + leftPad + rightPad, canvasHeight);

			// Paint it...
			if (!prefix && !postfix)
			{
				paint(canvas, (canvas.image_width() - textCanvas.image_width()) / 2, (canvas.image_height() - textCanvas.image_height()) / 2, textCanvas, 0, 0, textCanvas.image_width(), textCanvas.image_height(), Overwrite);
			}
			else if (!prefix && postfix)
			{
				int xPos = leftPad + prefixSpacing;
				int yPos = (canvasHeight - textCanvas.image_height()) / 2;
				paint(canvas, xPos, yPos, textCanvas, 0, 0, textCanvas.image_width(), textCanvas.image_height(), Overwrite);

				xPos = xPos + textCanvas.image_width() + postfixSpacing;
				yPos = (canvasHeight - postfix->image_height()) / 2;
				paint(canvas, xPos, yPos, *postfix, 0, 0, postfix->image_width(), postfix->image_height(), Overwrite);
			}
			else if (prefix && !postfix)
			{
				int xPos = leftPad;
				int yPos = (canvasHeight - prefix->image_height()) / 2;
				paint(canvas, xPos, yPos, *prefix, 0, 0, prefix->image_width(), prefix->image_height(), Overwrite);

				xPos = xPos + prefix->image_width() + prefixSpacing;
				yPos = (canvasHeight - textCanvas.image_height()) / 2;
				paint(canvas, xPos, yPos, textCanvas, 0, 0, textCanvas.image_width(), textCanvas.image_height(), Overwrite);
			}
			else
			{
				int xPos = leftPad;
				int yPos = (canvasHeight - prefix->image_height()) / 2;
				paint(canvas, xPos, yPos, *prefix, 0, 0, prefix->image_width(), prefix->image_height(), Overwrite);

				xPos = xPos + prefix->image_width() + prefixSpacing;
				yPos = (canvasHeight - textCanvas.image_height()) / 2;
				paint(canvas, xPos, yPos, textCanvas, 0, 0, textCanvas.image_width(), textCanvas.image_height(), Overwrite);

				xPos = xPos + textCanvas.image_width() + postfixSpacing;
				yPos = (canvasHeight - postfix->image_height()) / 2;
				paint(canvas, xPos, yPos, *postfix, 0, 0, postfix->image_width(), postfix->image_height(), Overwrite);
			}

			return canvas;
		}
	}
}

#endif // GWAIN_ARTIST_TEXT_HPP