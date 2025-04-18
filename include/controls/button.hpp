#ifndef GWAIN_CONTROLS_BUTTON_HPP
#define GWAIN_CONTROLS_BUTTON_HPP

#include "..\core\control.hpp"

namespace gwain
{
	namespace controls
	{
		// Create a button...
		class button : public control
		{
		public:

			typedef std::function<bool(mouse_button_id button)> OnClickCallback;

			button(const control_id& name, OnClickCallback onClickCallback = nullptr)
				: control(name)
			{
				button_mouse_over_texture_ = appearance_.texture_;
				button_disabled_texture_ = appearance_.texture_;
			}

			button(const control_id& name, const image& image, OnClickCallback onClickCallback = nullptr)
				: control(name, image), onClick_(onClickCallback)
			{
				button_mouse_over_texture_ = appearance_.texture_;
				button_disabled_texture_ = appearance_.texture_;
			}

			button(const control_id& name, const image& image, const gwain::image& imageMouseOver, OnClickCallback onClickCallback = nullptr)
				: control(name, image), button_mouse_over_texture_(std::make_shared<texture>(name + "_mouse_over_texture", imageMouseOver)), onClick_(onClickCallback)
			{
				button_disabled_texture_ = appearance_.texture_;
			}

			virtual bool button_click(mouse_button_id button) { return onClick_ ? onClick_(button) : propagate; }
			void button_click(const OnClickCallback& onClickCallback) { onClick_ = onClickCallback; }

			void button_enabled(bool enabled) { enabled_ = enabled; }

			std::shared_ptr<texture> button_mouse_over_texture() const { return button_mouse_over_texture_; }
			void button_mouse_over_texture(const std::shared_ptr<texture>& texture) { button_mouse_over_texture_ = texture; }

			std::shared_ptr<texture> button_disabled_texture() const { return button_disabled_texture_; }
			void button_disabled_texture(const std::shared_ptr<texture>& texture) { button_disabled_texture_ = texture; }

			const gwain::texture& control_texture() const
			{
				if (enabled_)
					return control_mouse_over() ? *button_mouse_over_texture_ : control::control_texture();
				return *button_disabled_texture_;
			}

			bool mouse_down(view& view, mouse_button_id button, const coord& xy) { return enabled_ ? button_click(button) : propagate; }

		protected:
			std::shared_ptr<gwain::texture> button_mouse_over_texture_;
			std::shared_ptr<gwain::texture> button_disabled_texture_;
		private:
			OnClickCallback onClick_ = nullptr;
			bool enabled_ = true;
		};
	}
}

#endif // GWAIN_CONTROLS_BUTTON_HPP