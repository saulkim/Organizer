#pragma once

#include "lastSinceEntries.h"
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>


class fileManager {

public:


	int readLastSinceFile();
	void readLastSinceFiletoData();
	void writeLastSinceToFile(std::vector<int> posOfCheckedButtons); // writes new ls entries to file
	std::vector<lastSinceEntries> getlsEntriesList();
	std::string getTimeDifferenceAsString(std::string time);
	std::string getCurrentTime();
	void shuffleFileName();

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