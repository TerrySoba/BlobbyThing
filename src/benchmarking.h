/*
 * benchmarking.h
 *
 *  Created on: 01.05.2012
 *      Author: yoshi252
 */

#ifndef BENCHMARKING_H_
#define BENCHMARKING_H_

#include "SDL.h"
#include <time.h>

class TakeTimeNow {
public:
    TakeTimeNow() {
        start = getTicks();
    }

    double tillNow() {
        return (getTicks() - start) / 1000000000.0;
    }

private:
    uint64_t getTicks() {
#ifdef _MSC_VER
        return SDL_GetTicks() * 1000000;
#else
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return ts.tv_sec * 1000000000 + ts.tv_nsec;
#endif
    }
    uint64_t start;
};


#endif /* BENCHMARKING_H_ */
