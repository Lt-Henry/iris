

#include "Window.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


Window::Window()
{
	set_title("Iris");
	set_border_width(12);
	set_position(Gtk::WIN_POS_CENTER); 
	
	box=Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	
	stack=Gtk::Stack();
	stack.set_transition_type(Gtk::STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	stack.set_transition_duration(1000);
	
	switcher = Gtk::StackSwitcher();
	switcher.set_stack(stack);
	
	
	box.pack_start(switcher,false,false,0);
	box.pack_start(stack,true,true,0);
	
	renderBox=Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	stack.add(renderBox,"Render","Render");
	btnRender = Gtk::Button("Render");
	renderBox.pack_start(btnRender,false,false,0);
	drawingArea = Gtk::DrawingArea();
	renderBox.pack_start(drawingArea,true,true,0);
	
	
	sourceBox=Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	stack.add(sourceBox,"Source","Source");
	sourceView=Gsv::View();
	sourceBox.pack_start(sourceView,true,true,0);
	
	add(box);
	
	show_all();
}


Window::~Window()
{
}
