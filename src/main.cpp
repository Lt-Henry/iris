
#include "Window.hpp"
#include "Parser.hpp"
#include "MeshLoader.hpp"

#include <gtkmm/application.h>

#include <iostream>
#include <string>
#include <stdexcept>


using namespace std;
using namespace com::toxiclabs::iris;


int main(int argc,char * argv[])
{
	cout<<"Iris path-tracer"<<endl;
	MeshLoader::LoadOBJ("crap");

	auto app = Gtk::Application::create(argc,argv,"com.toxiclabs.iris");

	Window mainWindow;
	
	return app->run(mainWindow);
}

