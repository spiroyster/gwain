#ifndef GWAIN_THEME_HPP
#define GWAIN_THEME_HPP

#include "instance.hpp"
#include "font.hpp"

namespace gwain
{
	namespace core
	{
		class default_theme : public instance_interface::theme_adaptor
		{
			// default label font...
			std::shared_ptr<font> default_font_;

			// default background colour...
			std::shared_ptr<texture> default_background_;

		public:
			default_theme()
				: default_background_(std::make_shared<texture>("default_theme_background", rgba32(255, 0, 0, 255)))
			{
				// load the default font...
				try
				{
					default_font_ = std::make_shared<font>(ttf_font("default.ttf", 24));
				}
				catch (const exception& e)
				{
					eos_dispatch::exception_thrown(e);
				}

				if (!default_font_)
					default_font_ = std::make_shared<font>(24, 24);
			}

			std::shared_ptr<texture> refresh(control* c, control* p = nullptr)
			{
				//if (auto clabel = dynamic_cast<label*>)

				return default_background_;
			}
		};
	}
	
}

#endif // GWAIN_THEME_HPP