#ifndef GWAIN_ARTIST_CAPTION_HPP
#define GWAIN_ARTIST_CAPTION_HPP

namespace gwain
{
	namespace artist
	{
		static std::shared_ptr<AxW::Draw::ImageRGBA32> CaptionImage(int leftPad, const AxW::Draw::ImageRGBA32* prefix,
			int prefixSpacing, const AxW::String& text, const AxW::Draw::Colour::RGBA32& textColour, const AxW::Draw::Font& font,
			int postfixSpacing, const AxW::Draw::ImageRGBA32* postfix, int rightPad)
		{
			auto textImage = AxW::Draw::Image::Text(font, text, font.MaxWidth() * static_cast<int>(text.size()), font.MaxHeight(), AxW::Draw::Image::TextAlignment::Centre, AxW::Draw::Image::TextAlignment::Centre);
			// We the need to crop the ends to tex width size..
			// Note the first and last x coords of pixels with alpha != 0...
			int xMin = textImage->Width();
			int xMax = 0;
			for (int y = 0; y < static_cast<int>(textImage->Height()); ++y)
				for (int x = 0; x < static_cast<int>(textImage->Width()); ++x)
				{
					if (static_cast<int>(textImage->Pixel(x, y)->A()) != 0)
					{
						if (x < xMin)
							xMin = x;
						if (x > xMax)
							xMax = x;
					}
				}

			xMax += 1;

			// Crop the text so that is only the required width...
			auto textCanvas = AxW::Draw::Image::SingleColour(xMax - xMin, textImage->Height(), AxW::Draw::Colour::RGBA32());
			textImage = AxW::Draw::Paint::Brush(static_cast<const AxW::Draw::ImageRGBA32&>(*textCanvas), 0, 0, *textImage, xMin, 0, textCanvas->Width(), textCanvas->Height(), AxW::Draw::Paint::Overwrite);

			AxW::Draw::Paint::Filter(*textImage, 0, 0, textImage->Width(), textImage->Height(), [&textColour](const AxW::Draw::Colour::RGBA32& imagePixel)
				{
					// Set the colour ...
					return imagePixel.A() ? AxW::Draw::Colour::RGBA32(textColour.R(), textColour.G(), textColour.B(), imagePixel.A()) : AxW::Draw::Colour::RGBA32();
				});


			unsigned int canvasWidth = (prefix ? prefix->Width() : 0) + prefixSpacing + textImage->Width() + postfixSpacing + (postfix ? postfix->Width() : 0);
			unsigned int canvasHeight = textImage->Height();
			if (prefix)
				canvasHeight = (std::max)(prefix->Height(), canvasHeight);
			if (postfix)
				canvasHeight = (std::max)(postfix->Height(), canvasHeight);

			// Create our canvas...
			auto canvas = AxW::Draw::Image::SingleColour(canvasWidth + leftPad + rightPad, canvasHeight, AxW::Draw::Colour::RGBA32());

			// Paint it...
			if (!prefix && !postfix)
			{
				AxW::Draw::Paint::Brush(*canvas, (canvas->Width() - textImage->Width()) / 2, (canvas->Height() - textImage->Height()) / 2, *textImage, 0, 0, textImage->Width(), textImage->Height(), AxW::Draw::Paint::Overwrite);
			}
			else if (!prefix && postfix)
			{
				int xPos = leftPad + prefixSpacing;
				int yPos = (canvasHeight - textImage->Height()) / 2;
				AxW::Draw::Paint::Brush(*canvas, xPos, yPos, *textImage, 0, 0, textImage->Width(), textImage->Height(), AxW::Draw::Paint::Overwrite);

				xPos = xPos + textImage->Width() + postfixSpacing;
				yPos = (canvasHeight - postfix->Height()) / 2;
				AxW::Draw::Paint::Brush(*canvas, xPos, yPos, *postfix, 0, 0, postfix->Width(), postfix->Height(), AxW::Draw::Paint::Overwrite);
			}
			else if (prefix && !postfix)
			{
				int xPos = leftPad;
				int yPos = (canvasHeight - prefix->Height()) / 2;
				AxW::Draw::Paint::Brush(*canvas, xPos, yPos, *prefix, 0, 0, prefix->Width(), prefix->Height(), AxW::Draw::Paint::Overwrite);

				xPos = xPos + prefix->Width() + prefixSpacing;
				yPos = (canvasHeight - textImage->Height()) / 2;
				AxW::Draw::Paint::Brush(*canvas, xPos, yPos, *textImage, 0, 0, textImage->Width(), textImage->Height(), AxW::Draw::Paint::Overwrite);
			}
			else
			{
				int xPos = leftPad;
				int yPos = (canvasHeight - prefix->Height()) / 2;
				AxW::Draw::Paint::Brush(*canvas, xPos, yPos, *prefix, 0, 0, prefix->Width(), prefix->Height(), AxW::Draw::Paint::Overwrite);

				xPos = xPos + prefix->Width() + prefixSpacing;
				yPos = (canvasHeight - textImage->Height()) / 2;
				AxW::Draw::Paint::Brush(*canvas, xPos, yPos, *textImage, 0, 0, textImage->Width(), textImage->Height(), AxW::Draw::Paint::Overwrite);

				xPos = xPos + textImage->Width() + postfixSpacing;
				yPos = (canvasHeight - postfix->Height()) / 2;
				AxW::Draw::Paint::Brush(*canvas, xPos, yPos, *postfix, 0, 0, postfix->Width(), postfix->Height(), AxW::Draw::Paint::Overwrite);
			}

			return canvas;
		}
	}
}

#endif // GWAIN_ARTIST_CAPTION_HPP