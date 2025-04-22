#ifndef GWAIN_VIEW_HPP
#define GWAIN_VIEW_HPP

#include "types.hpp"
#include "action.hpp"
#include "event.hpp"

#include <list>

namespace gwain
{
	class view
	{
	public:

		typedef std::function<void(const dim& region)> view_layout;
		typedef std::function<void(const dim& display)> view_show;
		typedef std::function<void()> view_hide;

		view(view_id name) : view_name_(name), view_region_(mask_hide) { eos_dispatch::view_registered(this); }
		
		view(view_id name, const std::vector<control*>& controls) : view_name_(name), view_region_(mask_hide) 
		{ 
			eos_dispatch::view_registered(this); 
			add(controls);
		}

		virtual ~view() { eos_dispatch::view_unregistered(this); }
		
		// aka resize...
		virtual void region(const dim& region)
		{ 
			eos_dispatch::view_resize(this);
			view_region_ = region;
		}

		const dim& region() const { return view_region_; }

		virtual void layout(const dim& region) { if (view_layout_) { view_layout_(region); }
		void layout(view_layout& callback) { view_layout_ = callback; }

		virtual void show(const dim& display) { if (view_show_) { view_show_(display); }
		void layout(view_show& callback) { view_show_ = callback; }

		virtual void hide() { if (view_hide_) { view_hide_(); }
		void hide(view_hide& callback) { view_hide_ = callback; }

		// add the control to this view...
		virtual void add(const control* cntl) { gwain::eos_dispatch::view_add_control(this, cntl); }

		void add(const std::vector<control*>& cntls) 
		{	
			for (auto& c : cntls)
				add(c);
		}

		virtual void remove(const control_id& cntl)
		{
			gwain::eos_dispatch::view_remove_control(this, cntl);
		}

		void remove(const std::vector<control*>& controls)
		{
			for (auto& c : controls)
				remove(c->control_name());
		}

		const view_id& view_name() const { return view_name_; }

	private:
		view_id view_name_;
		dim view_region_;

		view_layout view_layout_ = nullptr;
		view_show view_show_ = nullptr;
		view_hide view_hide_ = nullptr;
	};
}

#endif // GWAIN_VIEW_HPP
