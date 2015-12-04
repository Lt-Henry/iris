

#include "Window.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


Window::Window():
	box(Gtk::ORIENTATION_VERTICAL),
	renderBox(Gtk::ORIENTATION_VERTICAL),
	btnRender("Render"),
	sourceBox(Gtk::ORIENTATION_VERTICAL)
{
	set_title("Iris");
	set_border_width(12);
	set_position(Gtk::WIN_POS_CENTER); 
	

	
	stack.set_transition_type(Gtk::STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	stack.set_transition_duration(1000);
	
	switcher.set_stack(stack);
	
	
	box.pack_start(switcher,false,false,0);
	box.pack_start(stack,true,true,0);

	stack.add(renderBox,"Render","Render");
	
	//btnRender.set_hexpand(false);
	//btnRender.set_vexpand(false);
	btnRender.set_halign(Gtk::ALIGN_CENTER);
	drawingArea.set_size_request(640,480);
	
	renderBox.pack_start(btnRender,false,false,6);
	renderBox.pack_start(drawingArea,true,true,6);
	
	

	stack.add(sourceBox,"Source","Source");

	sourceBox.pack_start(sourceView,true,true,6);
	
	add(box);
	
	
	//signals
	
	drawingArea.signal_draw().connect(sigc::mem_fun(*this,&Window::OnDraw));
	btnRender.signal_clicked().connect(sigc::mem_fun(*this,&Window::OnBtnRenderClicked));
	show_all();
}


Window::~Window()
{
}

void Window::OnBtnRenderClicked()
{
	cout<<"Rendering..."<<endl;
}

bool Window::OnDraw(const Cairo::RefPtr<Cairo::Context> & cr)
{
	cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);
	cr->paint();
	
	return false;
}
