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


		virtual void layout(const dim& region) {}
		virtual void show(const dim& display) {}
		virtual void hide() {}



		/*void push(const std::shared_ptr<action_interface>& action) 
		{
			action_stack_.emplace_back(action);
			eos_dispatch::view_action_push(this, action.get());
		}

		std::shared_ptr<action_interface> pop() 
		{
			auto popped = action_stack_.back();
			action_stack_.pop_back();
			eos_dispatch::view_action_remove(this, popped.get());
			return popped;
		}

		virtual void remove(const action_interface_id& id) 
		{
			auto itr = std::find_if(action_stack_.begin(), action_stack_.end(), [=](const std::shared_ptr<action_interface>& ai) { return ai->action_interface_name() == id; });
			if (itr != action_stack_.end())
			{
				action_stack_.erase(itr);
				eos_dispatch::view_action_remove(this, itr->get());
			}
		}*/

		virtual void add(const control* cntl)
		{
			// add the control to this view...
			gwain::eos_dispatch::view_add_control(this, cntl);
		}

		void add(const std::vector<control*>& cntls)
		{
			// add the control to this view...
			for (auto& c : cntls)
				add(c);
		}

		virtual void remove(const control_id& cntl)
		{
			gwain::eos_dispatch::view_remove_control(this, cntl);
		}

		void remove(const std::vector<control*>& controls)
		{
			// add the control to this view...
			for (auto& c : controls)
				remove(c->control_name());
		}

		const view_id& view_name() const { return view_name_; }

	private:
		std::list<std::shared_ptr<action_interface>> action_stack_;
		view_id view_name_;
		dim view_region_;
	};
}

#endif // GWAIN_VIEW_HPP
