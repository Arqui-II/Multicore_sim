/*
 * Constants.h
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <limits.h>
#include <string>

namespace cons {

constexpr int BASE_TIME = 2;
constexpr int NUMBER_OF_CORES = 2;

namespace inst { //Instructions
enum TYPES {
	READ, CALC, WRITE
};

constexpr int MAX_DATA = USHRT_MAX;
constexpr int MAX_INST = 20;
}

namespace multipliers { //Frecuency multipliers
constexpr int CALC = 1;
constexpr int READ = 2;
constexpr int WRITE = 3;
constexpr double L1 = 0.5;
constexpr int L2 = 1;
constexpr double RAM = 1.5;
}

namespace memory { //Memory
enum L1_STATES {
	M, S, I
};
enum L2_STATES {
	DM, DS, DI
};

constexpr int L1_NUMBER = 2;
constexpr int L1_SIZE = 2;
constexpr int L2_SIZE = 4;
constexpr int RAM_SIZE = 16;
}

namespace bus { //Buses
enum EVENTS {
	READ_MISS, WRITE_MISS
};
}

namespace logger {
constexpr const char *TYPES[] = { "READ", "CALC", "WRITE" };
}

}

#endif /* CONSTANTS_H_ */
