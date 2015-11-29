

#include "Window.hpp"

#include <iostream>

using namespace com::toxiclabs::iris;
using namespace std;


Window::Window()
{
	set_title("Iris");
	set_window_width(12);
	
	show_all();
}


Window::~Window()
{
}
