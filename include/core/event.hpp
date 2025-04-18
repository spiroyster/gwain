#ifndef GWAIN_CORE_EVENT_HPP
#define GWAIN_CORE_EVENT_HPP

#include "eos.hpp"

namespace gwain
{
	class view;
	class control;
	class texture;
	class action_interface;
	class exception;

	namespace core
	{
		class control_interface;
	}

	EOS_EVENT(session_kill);
	EOS_EVENT_1(exception_thrown, const exception&, e);
	EOS_EVENT_1(view_registered, const view*, v);
	EOS_EVENT_1(view_unregistered, const view*, v);
	EOS_EVENT_1(view_resize, const view*, v);
	EOS_EVENT_2(view_add_control, const view*, v, const control*, s);
	EOS_EVENT_2(view_remove_control, const view*, v, const control_id&, s);
	EOS_EVENT_1(texture_registered, const texture*, t);
	EOS_EVENT_1(texture_unregistered, const texture*, t);
	EOS_EVENT_1(texture_image, const texture*, t);
	EOS_EVENT_1(control_registered, const control*, c);
	EOS_EVENT_1(control_unregistered, const control*, c);
	EOS_EVENT_1(capture_start, const control*, c);
	EOS_EVENT_1(capture_end, const control*, c);
	EOS_EVENT_2(view_action_push, const view*, v, const action_interface*, a);
	EOS_EVENT_2(view_action_remove, const view*, v, const action_interface*, a);

	
	
}

#endif // GWAIN_CORE_EVENT_HPP