#ifndef GWAIN_CONTROLS_LABEL_HPP
#define GWAIN_CONTROLS_LABEL_HPP

#include "..\core\control.hpp"
#include "..\artist\text.hpp"

namespace gwain
{
	namespace controls
	{
		// Create a button...
		class label : public gwain::control
		{
			image create_image(const std::wstring& text, const font& font, const rgba32& colour, const rgba32& background)
			{
				return artist::text(font, text, 0, 0, artist::text_alignment::centre, artist::text_alignment::centre);
			}

			std::wstring text_;
			rgba32 colour_;
			rgba32 background_;
			std::shared_ptr<font> font_;

		public:

			label(const control_id& name, const std::wstring& text)
				: gwain::control(name), text_(text)
			{
			}

			label(const control_id& name, const std::wstring& text, const std::shared_ptr<font>& font, const rgba32& colour, const rgba32& background)
				:	gwain::control(name, create_image(text, *font, colour, background)), text_(text), font_(font), colour_(colour), background_(background)
			{
			}

			void label_text(const std::wstring& text)
			{
				text_ = text;
				appearance_.texture_->texture_image(create_image(text_, *font_, colour_, background_));
			}

			const std::wstring& label_text() const { return text_; }

			void label_colour(const rgba32& colour)
			{
				colour_ = colour;
				appearance_.texture_->texture_image(create_image(text_, *font_, colour_, background_));
			}

			const rgba32& label_colour() const { return colour_; }

			void label_font(std::shared_ptr<font>& font)
			{
				font_ = font;
				appearance_.texture_->texture_image(create_image(text_, *font_, colour_, background_));
			}

			const std::shared_ptr<font>& label_font() const { return font_; }
		};
	}
}

#endif // GWAIN_CONTROLS_LABEL_HPP