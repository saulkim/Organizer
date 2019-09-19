#pragma once
#include <iostream>
#include <vector>

struct lastSinceEntries {
	std::string name;
	std::string lastTimeDoneAsSeconds; //cast into a long double so I don't get bothered with epoch time getting reset? about 2038 for int, using double for decimal precision incase of idk
	
	//std::vector<std::string> tagList; made into std::string for easier text display
	std::string tagList;

};