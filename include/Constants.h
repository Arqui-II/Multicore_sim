/*
 * Constants.h
 *
 *  Created on: 22 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <limits.h>

namespace cons {

constexpr int BASE_TIME = 2;

namespace inst {
enum TYPES {
	READ, CALC, WRITE
};

constexpr int MAX_DATA = USHRT_MAX;
constexpr int MAX_INST = 20;
}

namespace multipliers {
constexpr int CALC = 1;
constexpr int READ = 3;
constexpr int WRITE = 4;
constexpr double L1 = 0.5;
constexpr int L2 = 1;
constexpr double RAM = 1.5;
}

namespace memory {
enum L1_STATES {
	M, S, I
};
enum L2_STATES {
	DM, DS, DI
};

constexpr int L1_SIZE = 2;
constexpr int L2_SIZE = 4;
constexpr int RAM_SIZE = 16;
}

}

#endif /* CONSTANTS_H_ */
