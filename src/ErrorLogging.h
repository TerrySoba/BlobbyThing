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

#define _MY_STR_HELPER(x) #x
#define _MY_STR_(x) _MY_STR_HELPER(x)
#define _LOG_PREFIX __FILE__ ":" _MY_STR_(__LINE__) "> "

#define LOG(...)  ErrorLogging::getInstance()->log(LOGGING, _LOG_PREFIX, toStrVariadic(__VA_ARGS__))
#define WARN(...) ErrorLogging::getInstance()->log(WARNING, _LOG_PREFIX, toStrVariadic(__VA_ARGS__))
#define ERR(...)  ErrorLogging::getInstance()->log(ERROR,   _LOG_PREFIX, toStrVariadic(__VA_ARGS__))

enum LogLevel {
	LOGGING,
	WARNING,
	ERROR
};

template<typename T>
std::string toStrVariadic(T value) {
	return (boost::format("%1%") % value).str();
}

template<typename T, typename... Args>
std::string toStrVariadic(T value, Args... args) {
	return (boost::format("%1%%2%") % value % toStrVariadic(args...)).str();
}


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
 *  If you want to output multiple values just enter them separated by commas
 *
 *  Example:
 *  \code
 *  LOG("Variable someValue has value ", someValue);
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
