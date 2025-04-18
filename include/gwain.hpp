#ifndef GWAIN_HPP
#define GWAIN_HPP

#include "core/types.hpp"
#include "core/instance.hpp"

namespace gwain
{
	// render a view...
	void render_frame()
	{
		gwain::instance().render_frame();
	}

	void resize(int width, int height)
	{
		gwain::instance().resize(width, height);
	}

	// post an action...
	void post(const gwain::action_id& id, int value, const gwain::coord& display_coordinate)
	{
		gwain::instance().post(id, value, display_coordinate);
	}

	void show(view& v)
	{
		gwain::instance().show(v);
	}

	void hide(view& v)
	{
		gwain::instance().hide(v);
	}

	// find out which control is at xy...
	std::pair<view*, control*> interrogate(const coord& display_coordinate) 
	{
		return gwain::instance().interrogate(display_coordinate);
	}

	void init()
	{
		//instance();
		instance().default_interrogator(std::make_shared<core::default_interrogation>());
		instance().default_theme(std::make_shared<core::default_theme>());
	}

	template <class theme_class>
	void init()
	{
		//instance();
		instance().default_interrogator(std::make_shared<core::default_interrogation>());
		instance().default_theme(std::make_shared<theme_class>());
	}

	template <class theme_class, class interrogator_class>
	void init()
	{
		instance();
		instance().default_interrogator(std::make_shared<interrogator_class>());
		instance().default_theme(std::make_shared<theme_class>());
	}
}

#endif // GWAIN_HPP

