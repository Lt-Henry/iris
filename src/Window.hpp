
#ifndef _IRIS_WINDOW_
#define _IRIS_WINDOW_

#include <gtkmm.h>
#include <gtksourceviewmm.h>

namespace com
{
	namespace toxiclabs
	{
		namespace iris
		{
			class Window : public Gtk::Window
			{
				public:
				
				/* main layout */
				Gtk::Box box;
				Gtk::Stack stack;
				Gtk::StackSwitcher switcher;
				
				/* render layout */
				Gtk::Box renderBox;
				Gtk::DrawingArea drawingArea;
				Gtk::Button btnRender;
				
				/* source layout */
				Gtk::Box sourceBox;
				Gsv::View sourceView;
				
				
				void OnBtnRenderClicked();
				bool OnDraw(const Cairo::RefPtr<Cairo::Context> & cr);
				
				Window();
				virtual ~Window();
			};
		}
	}
}

#endif
