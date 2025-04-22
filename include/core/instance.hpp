#ifndef GWAIN_INSTANCE_HPP
#define GWAIN_INSTANCE_HPP

#include "control.hpp"
#include "event.hpp"
#include "view.hpp"
#include "theme.hpp"
#include "font.hpp"

#include <memory>
#include <iostream>

namespace gwain
{
	namespace core
	{
		class instance_interface :
			public eos_observer::view_registered,
			public eos_observer::view_unregistered,
			public eos_observer::control_registered,
			public eos_observer::control_unregistered,
			public eos_observer::view_add_control,
			public eos_observer::view_remove_control,
			public eos_observer::capture_start,
			public eos_observer::capture_end
		{
		public:

			class display_adaptor
			{
			public:
				virtual void render_frame(const dim& display, const std::vector<view*>& views) = 0;
			};

			class interrogation_adaptor
			{
			public:
				typedef std::pair<view*, control*> interrogated_control;
				virtual std::pair<view*, control*> interrogate(const coord& display_coordinate, const std::vector<view*>& views) = 0;
			};

			class theme_adaptor
			{
			public:
				virtual std::shared_ptr<texture> refresh(control* c, control* p = nullptr) = 0;
			};

			instance_interface(std::shared_ptr<display_adaptor> display, std::shared_ptr<interrogation_adaptor> interrogator)
				: display_adaptor_(display), interrogation_adaptor_(interrogator)
			{
			}

			void default_theme(const std::shared_ptr<theme>& th) { default_theme_ = th; }
			const std::shared_ptr<theme>& default_theme() { return default_theme_; }
			void default_interrogator(const std::shared_ptr<interrogation_adaptor>& ia) { interrogation_adaptor_ = ia; }

			// action event...
			void post(const action_id& id, int value, const coord& xy)
			{
				// interrogate...
				auto active_control = interrogation_adaptor_->interrogate(xy, active_views_);

				// if we have a capture...
				if (capture_control_.first)
					current_control_ = capture_control_;
				else
				{
					if (current_control_.second != active_control.second)
					{
						if (current_control_.second)
							current_control_.second->control_leave(xy);
						
						// if there was a tool tip, remove it...

						current_control_ = active_control;
						if (current_control_.second)
							current_control_.second->control_enter(xy);
					}
				}
				
				// Post the action event...
				if (current_control_.second)
				{
					switch (id)
					{
					case GWAIN_ACTION_ENTER:
						break;
					case GWAIN_ACTION_LEAVE:
						break;
					case GWAIN_ACTION_MOUSE_DOWN:
						current_control_.second->mouse_down(*current_control_.first, value, xy);
						break;
					case GWAIN_ACTION_MOUSE_UP:
						current_control_.second->mouse_up(*current_control_.first, value, xy);
						break;
					case GWAIN_ACTION_MOUSE_MOVE:
						current_control_.second->mouse_move(*current_control_.first, value, xy);
						break;
					case GWAIN_ACTION_KEY_UP:
						current_control_.second->key_up(*current_control_.first, value, xy);
						break;
					case GWAIN_ACTION_KEY_DOWN:
						current_control_.second->key_down(*current_control_.first, value, xy);
						break;
					}
				}
			}

			void show(view& v)
			{
				// add view to active view...
				active_views_.emplace_back(&v);

				// call show...
				v.show(display_size_);
			}

			void hide(view& v)
			{
				// hide the view...
				v.hide();

				// remove from active views...
				active_views_.erase(std::remove_if(active_views_.begin(), active_views_.end(),
					[=](view* vw) { return vw->view_name() == v.view_name(); }), active_views_.end());
			}

			// display resize...
			void resize(int width, int height)
			{
				display_size_ = { width, height };

				// resize the active views...
				//for (auto& v : active_views_)
				//	v->layout(display_size_, active_views_);
			}

			void render_frame()
			{
				if (display_adaptor_)
					display_adaptor_->render_frame(display_size_, active_views_);
			}

			std::pair<view*, control*> interrogate(const coord& display_coordinate)
			{
				if (interrogation_adaptor_)
					return interrogation_adaptor_->interrogate(display_coordinate, active_views_);
				return { nullptr, nullptr };
			}

			const std::list<const control*>& controls(view_id id) const
			{
				auto itr = view_controls_.find(id);
				return itr != view_controls_.end() ? itr->second : view_controls_empty_;
			}

			void view_registered_event(const view* view)
			{
				auto itr = views_.find(view->view_name());
				if (itr != views_.end())
					throw exception(error::viewAlreadyRegistered, view->view_name());

				views_[view->view_name()] = const_cast<gwain::view*>(view);
			}

			void view_unregistered_event(const view* view)
			{
				auto itr = views_.find(view->view_name());
				if (itr != views_.end())
					views_.erase(itr);

				// remove the control list for this view...
				view_controls_.erase(std::remove_if(view_controls_.begin(), view_controls_.end(),
					[=](gwain::view* vw) { return vw->view_name() == view->view_name(); }), view_controls_.end());
				
				// remove from active views...
				active_views_.erase(std::remove_if(active_views_.begin(), active_views_.end(),
					[=](gwain::view* vw) { return vw->view_name() == view->view_name(); }), active_views_.end());
			}

			void control_registered_event(const control* control)
			{
				auto itr = controls_.find(control->control_name());
				if (itr != controls_.end())
					throw exception(error::controlAlreadyRegistered, control->control_name());

				controls_[control->control_name()] = const_cast<gwain::control*>(control);;
			}

			void control_unregistered_event(const control* control)
			{
				auto itr = controls_.find(control->control_name());
				if (itr != controls_.end())
					controls_.erase(itr);

				// Remove from any view...
				for (auto vItr = view_controls_.begin(); vItr != view_controls_.end(); ++vItr)
					vItr->second.remove_if([=](const gwain::control* c) { return c->control_name() == control->control_name(); });
			}

			void view_add_control_event(const view* view, const control* control)
			{
				auto vItr = view_controls_.find(view->view_name());
				if (vItr != view_controls_.end())
				{
					auto cItr = std::find_if(vItr->second.begin(), vItr->second.end(), [=](const gwain::control* c) { return c->control_name() == control->control_name(); });
					if (cItr != vItr->second.end())
						throw exception(error::viewAlreadyHasControl, view->view_name() + ":" + control->control_name());
					vItr->second.emplace_back(control);
				}
				else
					throw exception(error::viewNotFound, view->view_name());
			}

			void view_remove_control_event(const view* view, const control* control)
			{
				auto vItr = view_controls_.find(view->view_name());
				if (vItr != view_controls_.end())
					vItr->second.remove_if([=](const gwain::control* c) { return c->control_name() == control->control_name(); });
				else
					throw exception(error::viewNotFound, view->view_name());
			}

		private:
			std::map<view_id, view*> views_;
			std::map<control_id, control*> controls_;
			std::map<texture_id, std::shared_ptr<texture>> textures_;

			std::vector<view*> active_views_;
			std::map<view_id, std::list<const control*>> view_controls_;
			std::list<const control*> view_controls_empty_;
			
			interrogation_adaptor::interrogated_control current_control_ = { nullptr, nullptr };
			interrogation_adaptor::interrogated_control capture_control_ = { nullptr, nullptr };

			dim display_size_;

			std::shared_ptr<display_adaptor> display_adaptor_ = nullptr;
			std::shared_ptr<interrogation_adaptor> interrogation_adaptor_ = nullptr;
			std::shared_ptr<theme> default_theme_ = nullptr;
		};

	}

	static core::instance_interface& instance();

}

#define GWAIN_DISPLAY(display) EOS_INIT \
namespace gwain\
{\
	namespace core\
	{\
		static std::unique_ptr<instance_interface> instance_; \
	}\
	core::instance_interface& instance()\
	{\
		if (!core::instance_) \
			core::instance_ = std::make_unique<core::instance_interface>(std::make_shared<display>()); \
		return *core::instance_; \
	}\
}

#endif // GWAIN_INSTANCE_HPP