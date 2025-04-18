#ifndef GWAIN_CORE_CONTROL_HPP
#define GWAIN_CORE_CONTROL_HPP

#include "types.hpp"
#include "action.hpp"
#include "appearance.hpp"
#include "event.hpp"

#include <list>

namespace gwain
{
	class tip;
	class image;

	class control : public action_interface
	{
	public:
		control(const control_id& name)
			:	control_name_(name),
				mask_(mask_auto)
		{
			eos_dispatch::control_registered(this);
			control_region(region_auto);
		}

		control(const control_id& name, const appearance& appearance)
			:	control_name_(name),
				appearance_(appearance),
				mask_(mask_auto)
		{
			eos_dispatch::control_registered(this);
			control_region(region_auto);
		}

		control(const control_id& name, const image& image)
			:	control_name_(name),
				mask_(mask_auto)
		{
			eos_dispatch::control_registered(this);
			appearance_.texture_ = std::make_shared<texture>(name + "_texture", image);
			control_region(region_auto);
		}

		control(const control_id& name, const appearance& appearance, const dim& region)
			:	control_name_(name),
				appearance_(appearance),
				mask_(mask_auto)
		{
			eos_dispatch::control_registered(this);
			control_region(region);
		}

		virtual ~control()
		{
			eos_dispatch::control_unregistered(this);
		}

		void control_parent(control* parent)
		{
			parent_ = parent;

			mouseMoveHandler_ = parent;
			mouseDownHandler_ = parent;
			mouseUpHandler_ = parent;
			mouseClickHandler_ = parent;
			mouseScrollHandler_ = parent;
			keyUpHandler_ = parent;
			keyDownHandler_ = parent;
			overHandler_ = parent;
		}
		control* control_parent() const { return parent_; }

		bool control_mouse_over() const
		{
			if (parent_)
				return parent_->mouse_over_flag_;
			return mouse_over_flag_;
		}

		void control_capture_start() { eos_dispatch::capture_start(this); }
		void control_capture_end() { eos_dispatch::capture_end(this); }

		const action_interface_id& action_interface_name() const { return control_name_; }
		const control_id& control_name() const { return control_name_; }

		bool mouse_move(view& view, mouse_button_id button, const coord& xy) { return mouseMoveHandler_ ? mouseMoveHandler_->mouse_move(view, button, xy) : action_interface::mouse_move(view, button, xy); }
		void mouse_move(control* handler) { mouseMoveHandler_ = handler; }

		bool mouse_down(view& view, mouse_button_id button, const coord& xy) { return mouseDownHandler_ ? mouseDownHandler_->mouse_down(view, button, xy) : action_interface::mouse_down(view, button, xy); }
		void mouse_down(control* handler) { mouseDownHandler_ = handler; }

		bool mouse_up(view& view, mouse_button_id button, const coord& xy) { return mouseUpHandler_ ? mouseUpHandler_->mouse_up(view, button, xy) : action_interface::mouse_up(view, button, xy); }
		void mouse_up(control* handler) { mouseUpHandler_ = handler; }

		bool mouse_scroll(view& view, int value, const coord& xy) { return mouseUpHandler_ ? mouseUpHandler_->mouse_scroll(view, value, xy) : action_interface::mouse_scroll(view, value, xy); }
		void mouse_scroll(control* handler) { mouseScrollHandler_ = handler; }

		bool key_up(view& view, int key, const coord& xy) { return keyUpHandler_ ? keyUpHandler_->key_up(view, key, xy) : action_interface::key_up(view, key, xy); }
		void key_up(control* handler) { keyUpHandler_ = handler; }

		bool key_down(view& view, int key, const coord& xy) { return keyDownHandler_ ? keyDownHandler_->key_down(view, key, xy) : action_interface::key_down(view, key, xy); }
		void key_down(control* handler) { keyDownHandler_ = handler; }

		void control_texture(const std::shared_ptr<texture>& texture) { appearance_.texture_ = texture; }
		void control_appearance(const appearance& appearance) { appearance_ = appearance; }

		void control_mask(const dim& m) { mask_ = m; }
		const dim& control_mask() const { return mask_; }

		void control_region(const dim& r) { control_resize(r); region_ = r; }
		const dim& control_region() const { return region_; }

		float control_depth() const { return zdepth_; }
		void control_depth(float depth) { zdepth_ = depth; }

		const coord& control_xy() const { return region_.xy_; }
		void control_xy(const coord& xy) { control_region({ xy, region_.wh_ }); }

		const coord& control_wh() const { return region_.wh_; }
		void control_wh(const coord& wh) { control_region({ region_.xy_, wh }); }

		// the current texture...
		virtual const texture& control_texture() const { return *appearance_.texture_; }

		// Set the layout xy/wh of this control...
		virtual void control_layout(const view& view) {}

		// called when control resized if new images need generating...
		virtual void control_resize(const dim& region);
		
		// called when control entered...
		virtual void control_enter(const coord& xy)
		{
			mouse_over_flag_ = true;
			if (overHandler_)
				overHandler_->control_enter(xy);
		}

		// called when control left...
		virtual void control_leave(const coord& xy)
		{
			mouse_over_flag_ = false;
			if (overHandler_)
				overHandler_->control_leave(xy);
		}

		// the tool tip for this control (if any)...
		virtual std::shared_ptr<tip> control_tip() const
		{
			if (overHandler_)
				return overHandler_->control_tip();
			return std::shared_ptr<tip>();
		}

	protected:
		appearance appearance_;
		control* parent_ = nullptr;
	private:

		// Handlers...
		control* mouseMoveHandler_ = nullptr;
		control* mouseDownHandler_ = nullptr;
		control* mouseUpHandler_ = nullptr;
		control* mouseClickHandler_ = nullptr;
		control* mouseScrollHandler_ = nullptr;
		control* keyUpHandler_ = nullptr;
		control* keyDownHandler_ = nullptr;
		control* overHandler_ = nullptr;

		control_id control_name_;
		bool mouse_over_flag_ = false;
		dim mask_;
		dim region_;
		float zdepth_ = 0.0f;
	};
}

#endif // GWAIN_CONTROL_HPP
