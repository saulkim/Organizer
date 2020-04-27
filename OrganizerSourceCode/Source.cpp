#include "organizerfluid.h"

int main(int argc, char** argv) {
	ShowWindow(GetConsoleWindow(), SW_HIDE); //hides the console window because this is a console program; alternate is to switch this over to a gui like win32 api or ... winmain()!
	mainGUI* letsgo = new mainGUI();
	letsgo->createWindow()->show(); //createWindow() returns a FL_window type so that can show() but letsgo-> wont work cuz mainGUI didnt inherit anything

	return Fl::run();
}