#include "organizerfluid.h"
#include "fileManager.h"
#include <FL/Fl_Text_Display.H>

bool isLSEditViewShowing = false; // warning! global variable?
bool dataFileExists = false; // warning! global variable?


void mainGUI::todoShow_cb(Fl_Widget* obj, void* p) {


	Fl_Group* consoleGroup = (  (Fl_Group*)  obj->parent()->parent()->child(3)    ); //gotta cast that obj->->-> as a group otherwise it will be known as a widget and won't have a child() function
	int numOfChildren = consoleGroup->children(); 
	for (int i = 0; i < numOfChildren; ++i) {
		consoleGroup->child(i)->hide();
	}
	//this todo box is child(0)
	consoleGroup->child(0)->show();

}
void mainGUI::lastSinceShow_cb(Fl_Widget* obj, void* p) {
	isLSEditViewShowing = false;

	Fl_Group* consoleGroup = (  (Fl_Group*)  obj->parent()->parent()->child(3)    ); //gotta cast that obj->->-> as a group otherwise it will be known as a widget and won't have a child() function
	
	int numOfChildren = consoleGroup->children();
	for (int i = 0; i < numOfChildren; ++i) {
		consoleGroup->child(i)->hide();
	}
	Fl_Group* lastSinceBoxGroup = (Fl_Group*)(consoleGroup->child(1));
	lastSinceBoxGroup->show();
	Fl_Group* lastSinceFileReadingErrorGroup = (Fl_Group*)(lastSinceBoxGroup->child(3));


	Fl_Group* outputViewGroup = (Fl_Group*)(lastSinceBoxGroup->child(1));
	Fl_Group* editViewGroup = (Fl_Group*)(lastSinceBoxGroup->child(2));
	editViewGroup->hide();

	Fl_Group* centerViewGroup = (Fl_Group*)(outputViewGroup->child(0));
	Fl_Group* longestLastSinceEntryGroup = (Fl_Group*)(centerViewGroup->child(1));


	Fl_Group* allLastSinceEntriesGroup = (Fl_Group*)(outputViewGroup->child(1));

	fileManager fileManagerLastSince;

	//check if directory exist, if true, skip ;; if false, create directory then continue
	if (fileManagerLastSince.does_Directory_Exist() == false) {

		//TODO check for sys permissions first so this doesnt result in crash error
		fileManagerLastSince.create_Directory();
	}

	int fileReadingStatus = fileManagerLastSince.readLastSinceFile();

	//if file reading works 0 yes, 1 = file not found, 2 = folder not found
	// TODO change this all into its own class


	if (fileReadingStatus == 0) {
		dataFileExists = true;
		outputViewGroup->show();
		fileManagerLastSince.readLastSinceFiletoData();
		std::vector<lastSinceEntries> lsEntriesVector = fileManagerLastSince.getlsEntriesList();

		//code for putting lsEntries into the rightside of the screen, all entries must be checkboxes
		allLastSinceEntriesGroup->clear();
		int lsEntryAmount = lsEntriesVector.size();
		int spacing = 0;

		Fl_Check_Button* newCheckBtn;
		std::string lsEntryName;
		std::string lsEntryTime;
		std::string lsEntryString;
		std::string timeDiff;
		const char* text;

		//actual checkbox creation time
		for (int i = 0; i < lsEntryAmount; ++i) {
			lsEntryName = "";
			lsEntryTime = "";
			lsEntryString = "";
			timeDiff = "";
			lsEntryName = lsEntriesVector[i].name;

			//finding the time difference between the entry and current time before adding to the GUI
			timeDiff = fileManagerLastSince.getTimeDifferenceAsString(lsEntriesVector[i].lastTimeDoneAsSeconds);
			lsEntryTime += timeDiff;
			lsEntryString += lsEntryName;
			lsEntryString += " ";
			lsEntryString += lsEntryTime;
			lsEntryString += " d";

			// warning! check to see if there's a memory leak here since the pointer is getting redirected, should not be a problem since I can access the checkbutton individually through the parent group
			newCheckBtn = new Fl_Check_Button(600, 130 + spacing, 140, 40);  

			text = lsEntryString.c_str();
			newCheckBtn->copy_label(text);
			allLastSinceEntriesGroup->add(newCheckBtn);
			spacing += 40;

		}

	} //end of successful file load

	else if (fileReadingStatus == 1) {
		dataFileExists = false;
		std::cout << "file reading status error 1";
		lastSinceFileReadingErrorGroup->label("no files exist,\n go click on + sign");
		lastSinceFileReadingErrorGroup->show();
	}

	else if (fileReadingStatus == 2) {
		dataFileExists = false;
		std::cout << "file reading status error 2";
		lastSinceFileReadingErrorGroup->label("no data folder exists, prolly a permission error. give exe admin privilege");
		lastSinceFileReadingErrorGroup->show();
	}



}
void mainGUI::batteryShow_cb(Fl_Widget* obj, void* p) {

	Fl_Group* consoleGroup = (  (Fl_Group*)  obj->parent()->parent()->child(3)    ); //gotta cast that obj->->-> as a group otherwise it will be known as a widget and won't have a child() function
	int numOfChildren = consoleGroup->children();
	for (int i = 0; i < numOfChildren; ++i) {
		consoleGroup->child(i)->hide();
	}
	//this battery box is child(2)
	consoleGroup->child(2)->show();

}
void mainGUI::lsUpdateBtn_cb(Fl_Widget* obj, void* p) {

	//Fl_Group* lastSinceBoxGroup = obj->parent()->parent()->parent()->parent();
	Fl_Group* mainWindowGroup = (Fl_Group*)(obj->parent()->parent()->parent()->parent()->parent()->parent()); //this is technically the fl_double_window but double window inherits from fl_group
	Fl_Group* toolBarGroup = (Fl_Group*)(mainWindowGroup->child(1));
	Fl_Button* lastSinceBtn = (Fl_Button*)(toolBarGroup->child(1));

	Fl_Group* outputViewGroup = (Fl_Group*)(obj->parent()->parent()->parent());
	Fl_Group* allLastSinceEntriesGroup = (Fl_Group*)(outputViewGroup->child(1)); //this is the righthand group box, no checkbuttons will show in gui builder since the checkbuttons are put in using my own code






	//loop over the righthand group and see which checkbuttons have value of 1
	std::vector<int> posOfCheckedButtons{}; // the [i] positions is either 0 for not checked or 1 for checked
	Fl_Check_Button* checkButton;


	for (int i = 0; i < allLastSinceEntriesGroup->children(); ++i) {
		checkButton = (Fl_Check_Button * )(allLastSinceEntriesGroup->child(i));
		if (checkButton->value() == 1) {
			posOfCheckedButtons.push_back(1);
		}
		else {
			posOfCheckedButtons.push_back(0);
		}
	}

	//warning! necessary for now to give scope to writeLastSinceToFile() for lsentries data since it can't access the one in lastsinceshow_cb. this is definitely a tech debt starting up. making file manager global gives its own error of crashing/double entries being written
	fileManager fileManagerInsidelsUpdateBtncb;
	fileManagerInsidelsUpdateBtncb.readLastSinceFile();
	fileManagerInsidelsUpdateBtncb.readLastSinceFiletoData();
	fileManagerInsidelsUpdateBtncb.writeLastSinceToFile(posOfCheckedButtons);
	fileManagerInsidelsUpdateBtncb.shuffleFileName();

	//redrawing the entire lsGroup view
	lastSinceShow_cb(lastSinceBtn, (void*) p); // warning! definitely a memory leak doing this probably

	//lastSinceBoxGroup->redraw();


}

void mainGUI::fanSpeedShow_cb(Fl_Widget* obj, void* p) {



}


void mainGUI::lsGearBoxBtn_cb(Fl_Widget* obj, void* p) {

	Fl_Double_Window* mainWindow = (Fl_Double_Window*)(obj->parent()->parent()->parent()->parent()->parent());
	Fl_Group* toolbarGroup = (Fl_Group*)(mainWindow->child(1));
	Fl_Button* lastSinceBtn = (Fl_Button*)(toolbarGroup->child(1));


	Fl_Group* lastSinceBoxGroup = (Fl_Group*)(obj->parent()->parent()->parent());
	Fl_Group* outputViewGroup = (Fl_Group*)(lastSinceBoxGroup->child(1));
	Fl_Group* editViewGroup = (Fl_Group*)(lastSinceBoxGroup->child(2));
	Fl_Group* lsFileReadingErrorViewGroup = (Fl_Group*)(lastSinceBoxGroup->child(3));



	//lsFileReadingErrorViewGroup->hide();
	if (isLSEditViewShowing == false) {
		outputViewGroup->hide();
		editViewGroup->show();
		lsFileReadingErrorViewGroup->hide();
		isLSEditViewShowing = true;
	}
	else if (isLSEditViewShowing == true && dataFileExists == true) {

		outputViewGroup->show();

		editViewGroup->hide();
		lsFileReadingErrorViewGroup->hide();
		isLSEditViewShowing = false;

		//redrawing the entire lsGroup view
		lastSinceShow_cb(lastSinceBtn, (void*)p); // warning! definitely a memory leak doing this probably
	}

	else if (isLSEditViewShowing == true && dataFileExists == false) {
		outputViewGroup->hide();
		editViewGroup->hide();
		lsFileReadingErrorViewGroup->show();
		isLSEditViewShowing = false;
	}


}

void mainGUI::lsEntryTextInputBtn_cb(Fl_Widget* obj, void* p) {
	
	Fl_Group* lsAddEntryGroup = (Fl_Group*)(obj->parent());
	Fl_Group* editView = (Fl_Group*)(lsAddEntryGroup->parent());
	Fl_Group* lsEditTextBottomView = (Fl_Group*)(editView->child(1));

	Fl_Input* lsEntryTextInput = (Fl_Input * )(lsAddEntryGroup->child(0));

	Fl_Output* lsEntryTextConfirmationOutput = (Fl_Output* )(lsEditTextBottomView->child(0));

	std::string entryName = lsEntryTextInput->value();

	
	//warning! main file + backup being overwritten to blank
	fileManager fileManagerAddingEntry;
	fileManagerAddingEntry.readLastSinceFile();
	fileManagerAddingEntry.readLastSinceFiletoData();
	fileManagerAddingEntry.writeLastSinceSingleEntryToFile(entryName);
	dataFileExists = true;

	// TODO add a function if input is not added successfully
	std::string outputString = entryName + " entered successfully";
	lsEntryTextConfirmationOutput->value(outputString.c_str());
	lsEntryTextInput->value("");
}