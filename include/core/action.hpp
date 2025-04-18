#ifndef GWAIN_CORE_ACTION_HPP
#define GWAIN_CORE_ACTION_HPP

#include "dim.hpp"

namespace gwain
{
	class view;

	class action_interface
	{
	public:

		static const bool CONSUME = true;		// action was handled...
		static const bool PROPAGATE = false;	// action was not handled...

		struct info
		{
			view& view_;
			int button_;
			coord xy_;
		};

		virtual bool mouse_move(view& view, mouse_button_id button, const coord& xy) { lastMouse_ = xy; return PROPAGATE; }
		virtual bool mouse_down(view& view, mouse_button_id button, const coord& xy) { lastMouse_ = xy; return PROPAGATE; }
		virtual bool mouse_up(view& view, mouse_button_id button, const coord& xy) { lastMouse_ = xy; return PROPAGATE; }
		virtual bool mouse_scroll(view& view, int value, const coord& xy) { lastMouse_ = xy; return PROPAGATE; }
		virtual bool key_up(view& view, int key, const coord& xy) { lastMouse_ = xy; return PROPAGATE; }
		virtual bool key_down(view& view, int key, const coord& xy) { lastMouse_ = xy; return PROPAGATE; }

		virtual const action_interface_id& action_interface_name() const = 0;
		virtual void draw(view& view) {}

		const coord& last_mouse() const { return lastMouse_; }

	private:
		coord lastMouse_;
	};

}

#endif // GWAIN_CORE_ACTION_HPP

