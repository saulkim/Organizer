#include "fileManager.h"
#include <map>



int fileManager::readLastSinceFile() { // 0 = successful, 1 = error
	int state;

	try {
		lsFile.open("./data/lastSinceEntry.txt", std::fstream::in);
		if (lsFile.good() == false) {
			std::cout << "state bad";
			state = 1;
			return state;
		}
		lsFile.clear();
		lsFile.close();

		state = 0;
	}

	catch (std::exception e) {
		std::cout << "error: readLastSinceFile()";
		state = 1;
	}


	return state;
}

void fileManager::readLastSinceFiletoData() {
	std::string line;
	lsFile.open("./data/lastSinceEntry.txt", std::fstream::in);
	if (lsFile.is_open()) {
		while (std::getline(lsFile, line)) {
			//parsing code here, im looping through every line name_time_tags,tags...
			std::string section = "name"; // section divided by '_' so there is name_time_tags 
			for (unsigned int i = 0; i < line.length(); ++i) {
				while (section == "name")
				{
					if (line[i] == '_') {
						section = "time";
						++i; //incrementing manually cuz break doesnt increment for loop?
						break;
					}
					else {
						lsEntries.name += line[i];
						break;
					}
				}				
				while (section == "time")
				{
					if (line[i] == '_') {
						section = "tags";
						++i; //incrementing manually cuz break doesnt increment
						break;
					}
					else {
						lsEntries.lastTimeDoneAsSeconds += line[i]; //still a string, must convert to long double later
						break;
					}
				}

				while (section == "tags")
				{
					if (line[i] == '_') {
						//this is the part at the end of the current line for the lastsinceEntry.txt, still in the loop of reading entire file

						lsEntriesList.push_back(lsEntries);
	
						section = "name";
						lsEntries.name = "";
						lsEntries.lastTimeDoneAsSeconds = "";
						lsEntries.tagList = "";
						++i;
						break;
					}
					else {
						


						lsEntries.tagList += line[i];
						break;
					}
				}
			} //for loop end, done parsing
		} // while loop end, done reading file
		//adding to lsEntriesList done
		lsFile.clear();

		lsFile.close();

		//sorting the lsEntries by largest epoch time(longest ago) to top of list 
		sortLSEntries();
	}
}



void fileManager::writeLastSinceToFile(std::vector<int> posOfCheckedButtons) { //format name_last time entry was done in epoch seconds_tag,tag,tag,..._

	//write temp file.txt here
	try{
		lsFile.open("./data/lastSinceEntryTemp.txt", std::fstream::out | std::fstream::trunc); //previous content deleted
		if (lsFile.good() == false) {
			throw "temp file creation error";
			lsFile.close();
			return;
		}
		//rewriting the checkbuttons marked for update
		for (unsigned int i = 0; i < lsEntriesList.size(); ++i) {
			if (posOfCheckedButtons[i] == 1) {
				lsFile << lsEntriesList[i].name << "_";

				//getting current epoch time to put in as string and +i so that there's no duplicate time
				std::string timeasString = getCurrentTime();
				long time = std::stol(timeasString);
				lsFile << std::to_string(time) << "_";

				lsFile << lsEntriesList[i].tagList << "_";
				lsFile << std::endl; //warning! windows does \n but unix based does \r\n?? so this will prolly unportable, when checking portability see if endl uses the system newline or a \n
			}
			else {

					lsFile << lsEntriesList[i].name << "_";
					lsFile << lsEntriesList[i].lastTimeDoneAsSeconds << "_";
					lsFile << lsEntriesList[i].tagList << "_";
					lsFile << std::endl; //warning! windows does \n but unix based does \r\n?? so this will prolly unportable, when checking portability see if endl uses the system newline or a \n
			}

		}
		lsFile.clear();
		lsFile.close();




	}
	catch (std::exception e) {
		std::cout << "error: file writing error";
	}
}


void fileManager::calcTimeDifference(std::string time) {
	
	long double timeToInt = stoi(time); //using long double because epoch may have problems in 2038 if using standard int32 .. using double for precision incase idk


	std::chrono::time_point<std::chrono::system_clock> now, lastTime, converterHelper;
	now = std::chrono::system_clock::now();

	lastTime = std::chrono::system_clock::from_time_t(timeToInt);
	converterHelper = std::chrono::system_clock::from_time_t(0); //easier to use to use the lib's converter functions

	std::chrono::duration<long double> nowTime = now - converterHelper;
	long double printtime = (long double)nowTime.count();
	
	std::chrono::duration<long double> elapsedTime = now - lastTime;
	timeDifference = (int)elapsedTime.count(); //casting as int for easier displaying since the entire epoch bit size isn't an issue after subtracting each other
}

std::string fileManager::getTimeDifferenceAsString(std::string time) {
	calcTimeDifference(time);

	timeDifference = secondsToDays(timeDifference);

	timeDifferenceAsString = std::to_string(timeDifference);
	return timeDifferenceAsString;
}


std::vector<lastSinceEntries> fileManager::getlsEntriesList() {
	return lsEntriesList;
}


int fileManager::secondsToDays(int seconds) {

	seconds = seconds / 60; //now in minutes
	seconds = seconds / 60; //now in hours
	seconds = seconds / 24; //now in days

	return seconds;
}

void fileManager::sortLSEntries() {
	// uses multimap so that i can sort by time value (the time is put as the key) and mutlimap instead of regular map so that i can have duplicate time
	//warning! didn't check all combination of different/same time so there might be a bug waiting to happen here
	//going lowest to highest epoch time since the lowest value since the lowest epoch time is the entry that was done pastest ago
	std::vector<lastSinceEntries> tempLSEntriesList;

	std::vector<long double> timeList;
	std::map<int, long double> originalTimeList;
	long double current = 0;


	std::multimap<long, std::pair<std::string, std::string>> orderedEntries;
	//making a map with the key = time, value = name of the entries, tags (all as one string apparently)
	for (unsigned int i = 0; i < lsEntriesList.size(); ++i) {
	
		current = stoi(lsEntriesList[i].lastTimeDoneAsSeconds); //since lastTimeDoneAsSeconds is a string

		orderedEntries.insert(std::pair<long , std::pair<std::string, std::string>>(current, std::pair<std::string, std::string>(lsEntriesList[i].name, lsEntriesList[i].tagList))); //long but is inserting map, value[is a pair]
	}

	//sorting the map by key (which has the numerical value)
	// maps are autosorted by key, no need for additional code

	//writing entries to temp in order
	tempLSEntriesList.clear();
	std::string tempName;
	std::string tempTime;
	std::string tempTags;
	lastSinceEntries tempLSE;
	for (auto const& element : orderedEntries) { // map < time, <name, tags> >  aka map < first, <second.first, second.second>>
		tempName = element.second.first;
		tempTime = std::to_string((long)element.first); //casting to long to truncate the decimal points from double
		tempTags = element.second.second;
		tempLSE.name = tempName;
		tempLSE.lastTimeDoneAsSeconds = tempTime;
		tempLSE.tagList = tempTags;

		tempLSEntriesList.push_back(tempLSE);
	}

	
	lsEntriesList = tempLSEntriesList;
	totalLSEntrySize = lsEntriesList.size();
}

std::string fileManager::getCurrentTime() {

	
	std::chrono::time_point<std::chrono::system_clock> now, converterHelper;
	now = std::chrono::system_clock::now();
	converterHelper = std::chrono::system_clock::from_time_t(0); //easier to use to use the lib's converter functions

	// using long double because epoch may have problems in 2038 if using standard int32 .. using double for precision incase idk
	std::chrono::duration<long double> nowTime = now - converterHelper; // not doing '- converterHelper' gives an error of "no suitable conversion"


	long double nowAsLD = (long double)nowTime.count();
	int nowAsInt = (int) nowAsLD; //warning! casting as int to throw away decimals from double, this is definitely gonna be a problem later. fix it when i learn more about how floats/doubles work
	 std::string nowAsString = std::to_string(nowAsInt);

	return nowAsString;
}

void fileManager::shuffleFileName() {
	// currently the original, new temp file, and backup (a copy of the original) exist
	// delete backup
	// rename original to backup, 
	// delete original
	// rename temp to original
	// delete temp
	int result; // 0 for success, 
	char ogName[] = "./data/lastSinceEntry.txt";
	char tempName[] = "./data/lastSinceEntryTemp.txt";
	char backupName[] = "./data/lastSinceEntryBackUp.txt";

	try {
		//todo check for errors/file integrity after each result
		result = std::remove(backupName);
		result = std::rename(ogName, backupName);
		result = std::remove(ogName);
		result = std::rename(tempName, ogName);
		result = std::remove(tempName);





	}

	catch (std::exception e) {
		std::cout << "shuffleFileName() error";
	}

}