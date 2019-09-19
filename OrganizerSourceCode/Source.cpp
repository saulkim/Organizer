#include "organizerfluid.h"

int main(int argc, char** argv) {
	mainGUI* letsgo = new mainGUI();
	letsgo->createWindow()->show(); //createWindow() returns a FL_window type so that can show() but letsgo-> wont work cuz mainGUI didnt inherit anything

	return Fl::run();
}