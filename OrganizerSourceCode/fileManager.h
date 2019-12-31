#pragma once

#include "lastSinceEntries.h"
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <filesystem>


class fileManager {

public:
	bool does_Directory_Exist(); // checks if the folder directory exists
	int readLastSinceFile();// 0 = successful, 1 = error .. only checking if files exist
	void readLastSinceFiletoData(); //reads the actual data
	void writeLastSinceToFile(std::vector<int> posOfCheckedButtons); // writes new ls entries to file
	void writeLastSinceSingleEntryToFile(std::string); //writes new ls entry from gearbox view to file
	std::vector<lastSinceEntries> getlsEntriesList();
	std::string getTimeDifferenceAsString(std::string time);
	std::string getCurrentTime();
	void shuffleFileName();
	void create_Directory(); // TODO maybe make this load in a parameter that checks to see how to make directory for that specific OS, just like in HMH


private:
	lastSinceEntries lsEntries;
	std::string lastSinceFileName = "lastSinceEntry";
	std::fstream lsFile;
	std::vector<lastSinceEntries> lsEntriesList;
	
	
	int totalLSEntrySize;
	int timeDifference;
	std::string timeDifferenceAsString;
	void calcTimeDifference(std::string time);
	int secondsToDays(int seconds);
	void sortLSEntries();

};