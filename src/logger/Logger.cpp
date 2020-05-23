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

void Logger::write(const std::string &pMessage) {
	_fileMutex.lock();
//	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//	std::stringstream ss;
//	ss << "[" << std::ctime(&now) << "] " << pMessage;
	std::cout << pMessage;
	_logFile << pMessage;
	_fileMutex.unlock();
}

