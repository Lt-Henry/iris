
#ifndef _IRIS_WINDOW_
#define _IRIS_WINDOW_

#include <gtkmm.h>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Window : public Gtk::Window
			{
				public:
				
				Gtk::Box box;
				Gtk::Stack stack;
				Gtk::StackSwitcher switcher;
				Gtk::DrawingArea drawingArea;
				Gtk::Label lblTest;
				Gtk::Button btnRender;
				
				Window();
				virtual ~Window();
			};
		}
	}
}

#endif
