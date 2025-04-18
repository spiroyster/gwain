#ifndef GWAIN_ARTIST_CHECKER_HPP
#define GWAIN_ARTIST_CHECKER_HPP

#include "..\core\image.hpp"

namespace gwain
{
	namespace artist
	{
		static image checker(ui width, ui height, const rgba32& colourA, ui uCount, const rgba32& colourB, ui vCount)
		{
			auto img = image(width, height);

			ui uStride = width / uCount;
			ui vStride = height / vCount;

			for (i p = 0; p < img.pixel_count(); ++p)
			{
				bool useColourA = true;
				ui x = p % width;
				ui y = static_cast<ui>(p / width);
				if ((x / uStride) % 2)
					useColourA = !useColourA;
				if ((y / vStride) % 2)
					useColourA = !useColourA;
				img[p] = useColourA ? colourA : colourB;
			}
			return img;
		}
	}
}

#endif // GWAIN_ARTIST_PALETTE_HPP