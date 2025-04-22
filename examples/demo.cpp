#include "..\include\shogl\shogl.hpp"

#include "..\include\gwain.hpp"
#include "..\include\platform\gl4.hpp"

GWAIN_DISPLAY(gwain::gl4)

#include "..\include\controls\button.hpp"
#include "..\include\controls\label.hpp"


//class myButton : public gwain::controls::button
//{
//	gwain::controls::label label_;
//public:
//	myButton(const gwain::control_id& name, const std::wstring& text)
//		:	gwain::controls::button(name),
//			label_(name + "_label", text)
//	{
//	}
//
//	myButton(const gwain::control_id& name, const std::wstring& text, gwain::controls::button::OnClickCallback callback)
//		:	gwain::controls::button(name, callback),
//			label_(name + "_label", text)
//	{
//	}
//
//	void layout()
//	{
//		// Centre the label wrt to the button...
//		//label_.control_region(gwain::dim_centred(label_.)
//	}
//};



int main(int argc, char** argv)
{
	try
	{
		// Inialise gwain (use OpenGL4... and default interrogator and default theme)...
		gwain::init();

		// Create some buttons...
		myButton button1("Button1", L"ClickMe1");
		button1.button_click([](gwain::mouse_button_id button) 
			{
				if (button == gwain::GWAIN_MOUSE_LEFT)
				{
					MessageBox(NULL, L"Button1", L"Msg", MB_ICONEXCLAMATION);
					return true;
				}
				return false;
			});

		myButton button2("Button2", L"ClickMe2", [](gwain::mouse_button_id button)
			{
				if (button == gwain::GWAIN_MOUSE_LEFT)
				{
					MessageBox(NULL, L"Button2", L"Msg", MB_ICONEXCLAMATION);
					return true;
				}
				return false;
			});

		// Create a view to display the buttons...
		gwain::view view("MyView", { &button1, &button2 });

		// callback layout view...
		view.layout([=](const gwain::dim& region) 
			{

			
			
			});





		// show the view...
		gwain::show(view);

		shogl()->draw([]()
			{
				glClear(GL_COLOR_BUFFER_BIT);
				gwain::render_frame();
			});

		shogl()->resize([](int width, int height)
			{
				gwain::resize(width, height);
			});

		shogl()->mouse_move([](int x, int y)
			{
				gwain::post(gwain::GWAIN_ACTION_MOUSE_MOVE, 0, { x, y });
			});

		shogl()->mouse_down([](int x, int y, shogl_window::mouse_button button)
			{
				switch (button)
				{
				case shogl_window::mouse_button::left:
					gwain::post(gwain::GWAIN_ACTION_MOUSE_DOWN, gwain::GWAIN_MOUSE_LEFT, { x, y });
					break;
				case shogl_window::mouse_button::middle:
					gwain::post(gwain::GWAIN_ACTION_MOUSE_DOWN, gwain::GWAIN_MOUSE_MIDDLE, { x, y });
					break;
				case shogl_window::mouse_button::right:
					gwain::post(gwain::GWAIN_ACTION_MOUSE_DOWN, gwain::GWAIN_MOUSE_RIGHT, { x, y });
					break;
				}
			});

		return shogl()->window_show();
	}
	catch (const gwain::exception& e)
	{
		gwain::eos_dispatch::exception_thrown(e);
	}

	return 1;
}