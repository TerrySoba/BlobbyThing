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
#include <fstream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using boost::shared_ptr;
using boost::make_shared;


#define checked_fread(dest, size, nmemb, fp) _checked_fread(dest, size, nmemb, fp, _LOG_PREFIX)

inline void _checked_fread(void* dest, size_t size, size_t nmemb, std::fstream& fp, const char* prefix) {
	fp.read(static_cast<char*>(dest), size * nmemb);
	if (fp.fail()) {
		WARN(prefix, " > read number of elements did not match requested number of elements.");
	}
}

#endif /* COMMON_H_ */
