/*
 * Logger.h
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <mutex>

#define FILE_NAME "/Sim_Log.txt"

class Logger {

private:

	std::mutex _fileMutex;
	std::string _logPath;
	std::ofstream _logFile;

public:
	Logger();
	virtual ~Logger();

	void write(const std::string &pMessage);
};

#endif /* LOGGER_H_ */
