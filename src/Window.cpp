

#include "Window.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


Window::Window()
{
	set_title("Iris");
	set_border_width(12);
	
	box=Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	
	stack=Gtk::Stack();
	stack.set_transition_type(Gtk::STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	stack.set_transition_duration(1000);
	
	switcher = Gtk::StackSwitcher();
	switcher.set_stack(stack);
	
	drawingArea = Gtk::DrawingArea();
	lblTest = Gtk::Label("Not implemented yet");
	
	stack.add(drawingArea,"Render","Render");
	stack.add(lblTest,"Source","Source");
	
	box.pack_start(switcher,true,true,0);
	box.pack_start(stack,true,true,0);
	
	btnRender = Gtk::Button("Render");
	box.pack_start(btnRender,true,true,0);
	
	add(box);
	
	show_all();
}


Window::~Window()
{
}
