#ifndef GWAIN_ARTIST_TRANSFORM_HPP
#define GWAIN_ARTIST_TRANSFORM_HPP

#include "..\core\image.hpp"

namespace gwain
{
	namespace transform
	{
        static void horizontal(image& img)
        {
            auto new_data = img.image_data();
            for (int y = 0; y < img.image_height(); ++y)
            {
                for (int x = 0; x < img.image_width(); ++x)
                {
                    int source = ((y * img.image_width()) + x) * img.pixel_channels();
                    int target = ((y * img.image_width()) + ((img.image_width() - 1) - x)) * img.pixel_channels();
                    for (unsigned int c = 0; c < static_cast<unsigned int>(img.pixel_channels()); ++c)
                        new_data[target + c] = img[source + c];
                }
            }
            img = image(img.image_width(), img.image_height(), new_data);
        }

        static void vertical(image& img)
        {
            auto new_data = img.image_data();
            for (int y = 0; y < img.image_height(); ++y)
            {
                for (int x = 0; x < img.image_width(); ++x)
                {
                    int source = ((y * img.image_width()) + x) * img.pixel_channels();
                    int target = ((((img.image_height() - 1) - y) * img.image_width()) + x) * img.pixel_channels();
                    for (unsigned int c = 0; c < static_cast<unsigned int>(img.pixel_channels()); ++c)
                        new_data[target + c] = img[source + c];
                }
            }
            img = image(img.image_width(), img.image_height(), new_data);
        }
	}
}


#endif // GWAIN_ARTIST_TRANSFORM_HPP