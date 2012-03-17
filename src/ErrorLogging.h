/*
 * ErrorLogging.h
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#ifndef ERRORLOGGING_H_
#define ERRORLOGGING_H_

#include <iostream>
#include <boost/format.hpp>
#include <string>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define _LOG_PREFIX __FILE__ ":" STR(__LINE__) "> "

#define fmt boost::format

#define LOG(text) ErrorLogging::getInstance()->log(LOGGING, _LOG_PREFIX, text)
#define WARN(text) ErrorLogging::getInstance()->log(WARNING, _LOG_PREFIX, text)
#define ERR(text) ErrorLogging::getInstance()->log(ERROR, _LOG_PREFIX, text)

enum LogLevel {
	LOGGING,
	WARNING,
	ERROR
};

/*! \brief General logging class.
 *
 *  This class is used to do log outputs. This class is implemented as a
 *  singleton, so you cannot instantiate it. To use it you have to use
 *  the getInstance() method. Usually this is not used directly, though.
 *  Usually the macros LOG(), WARN() and ERR() are used.
 *
 *  Example:
 *  \code
 *  LOG("Some logged message");
 *  \endcode
 *
 *  If you want to output some variables use boost::format.
 *
 *  Example:
 *  \code
 *  LOG(fmt("Variable someValue has value %1%") % someValue);
 *  \endcode
 */
class ErrorLogging {
public:

	static ErrorLogging* getInstance();
	virtual ~ErrorLogging();

	void log(LogLevel level, std::string logPrefix, std::string text);

	void log(LogLevel level, std::string logPrefix, boost::format& text) {
		log(level, logPrefix, text.str());
	}

protected:
	ErrorLogging();

private:
	static ErrorLogging* instance;
};

#endif /* ERRORLOGGING_H_ */
