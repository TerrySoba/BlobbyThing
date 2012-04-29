/*
 * common.h
 *
 *  Created on: 28.03.2012
 *      Author: yoshi252
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "ErrorLogging.h"
#include <memory>

using std::shared_ptr;
using std::make_shared;

#define checked_fread(dest, size, nmemb, fp) _checked_fread(dest, size, nmemb, fp, _LOG_PREFIX)

inline size_t _checked_fread(void* dest, size_t size, size_t nmemb, FILE* fp, const char* prefix) {
	size_t membRead = fread(dest, size, nmemb, fp);
	if (nmemb != membRead) {
		WARN(prefix, " > fread warning: read number of elements did not match requested number of elements. Requested:", nmemb, " Read:", membRead);
	}
	return membRead;
}

#endif /* COMMON_H_ */
