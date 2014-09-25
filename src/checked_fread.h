/*
 * common.h
 *
 *  Created on: 28.03.2012
 *      Author: yoshi252
 */

#ifndef CHECKED_FREAD_H_
#define CHECKED_FREAD

#include "ErrorLogging.h"
#include <fstream>

#define checked_fread(dest, size, nmemb, fp) _checked_fread(dest, size, nmemb, fp, _LOG_PREFIX)

inline void _checked_fread(void* dest, size_t size, size_t nmemb, std::fstream& fp, const char* prefix) {
    fp.read(static_cast<char*>(dest), size * nmemb);
    if (fp.fail()) {
        WARN(prefix, " > read number of elements did not match requested number of elements.");
    }
}

#endif
