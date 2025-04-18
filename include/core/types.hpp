#ifndef GWAIN_TYPES_HPP
#define GWAIN_TYPES_HPP

#include <string>
#include <variant>

namespace gwain
{
	// Fundamental types...
	typedef int i;
	typedef unsigned int ui;
	typedef float f;
	typedef unsigned char uc;
	
	// ID's
	typedef std::string event_id;
	typedef std::string control_id;
	typedef control_id action_interface_id;
	typedef std::string texture_id;
	typedef std::string exception_id;
	typedef std::string view_id;
	typedef std::string font_id;

	typedef ui action_id;
	typedef ui mouse_button_id;
	typedef ui key_id;

	// Consts...
	constexpr gwain::action_id GWAIN_ACTION_ENTER = 1;
	constexpr gwain::action_id GWAIN_ACTION_LEAVE = 2;
	constexpr gwain::action_id GWAIN_ACTION_MOUSE_MOVE = 3;
	constexpr gwain::action_id GWAIN_ACTION_MOUSE_DOWN = 4;
	constexpr gwain::action_id GWAIN_ACTION_MOUSE_UP = 5;
	constexpr gwain::action_id GWAIN_ACTION_MOUSE_CLICK = 6;
	constexpr gwain::action_id GWAIN_ACTION_KEY_UP = 7;
	constexpr gwain::action_id GWAIN_ACTION_KEY_DOWN = 8;

	constexpr gwain::mouse_button_id GWAIN_MOUSE_UNDEF = 0;
	constexpr gwain::mouse_button_id GWAIN_MOUSE_LEFT = 1;
	constexpr gwain::mouse_button_id GWAIN_MOUSE_RIGHT = 2;
	constexpr gwain::mouse_button_id GWAIN_MOUSE_MIDDLE = 3;



}

#endif // GWAIN_TYPES_HPP
