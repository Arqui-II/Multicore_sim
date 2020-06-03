/*
 * Logger.cpp
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <logger/Logger.h>

std::string getPath() {
	char temp[FILENAME_MAX];
	return (getcwd(temp, sizeof(temp)) ? std::string(temp) : std::string(""));
}

inline bool checkFile(const std::string &name) {
	std::ifstream f(name.c_str());
	return f.good();
}

Logger::Logger() {
	_logPath = getPath() + FILE_NAME;

	if (!checkFile(_logPath)) //Create file
		std::ofstream output(_logPath);
	else { //Remove and create new
		std::remove(_logPath.c_str());
		std::ofstream output(_logPath);
	}

	_logFile.open(_logPath); //Open file
}

Logger::~Logger() {
}

void Logger::write(std::string pMsg) {
	std::lock_guard<std::mutex> lock(_fileMutex);
	_logFile.open(_logPath); //Open file
	_logFile << pMsg.c_str();
}

