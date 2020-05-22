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

	if (!checkFile(_logPath)) std::ofstream output(_logPath);

	_logFile.open(_logPath);
}

Logger::~Logger() {
// TODO Auto-generated destructor stub
}

void Logger::write(const std::string &pMessage) {
	_fileMutex.lock();
	_logFile << pMessage;
	_fileMutex.unlock();
}

