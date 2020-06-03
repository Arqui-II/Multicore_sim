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

constexpr int BASE_TIME = 4;
constexpr int NUMBER_OF_CORES = 2;
constexpr int NUMBER_OF_CHIPS = 2;

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
constexpr double L1 = 1;
constexpr double L2 = 1.5;
constexpr double RAM = 2;
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
	READ_MISS, WRITE_MISS, READ_HIT, WRITE_HIT, WRITE_BACK
};
}

namespace logger {
constexpr const char *TYPES[] = { "READ", "CALC", "WRITE" };
constexpr const char *STATES_L1[] = { "M", "S", "I" };
constexpr const char *STATES_L2[] = { "DM", "DS", "DI" };
}

}

#endif /* CONSTANTS_H_ */
