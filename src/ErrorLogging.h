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
#include <sstream>

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

#ifdef _MSC_VER
// Microsoft Visual C++ does not support variadic templates, so we have to simulate them
template <typename A>
std::string toStrVariadic(const A& a) {
	std::stringstream oss;
	oss << a;
	return oss.str();
}

template <typename A, typename B>
std::string toStrVariadic(const A& a, const B& b) {
	std::stringstream oss;
	oss << a << b;
	return oss.str();
}

template <typename A, typename B, typename C>
std::string toStrVariadic(const A& a, const B& b, const C& c) {
	std::stringstream oss;
	oss << a << b << c;
	return oss.str();
}

template <typename A, typename B, typename C, typename D>
std::string toStrVariadic(const A& a, const B& b, const C& c, const D& d) {
	std::stringstream oss;
	oss << a << b << c << d;
	return oss.str();
}

template <typename A, typename B, typename C, typename D, typename E>
std::string toStrVariadic(const A& a, const B& b, const C& c, const D& d, const E& e) {
	std::stringstream oss;
	oss << a << b << c << d << e;
	return oss.str();
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
std::string toStrVariadic(const A& a, const B& b, const C& c, const D& d, const E& e, const F& f) {
	std::stringstream oss;
	oss << a << b << c << d << e << f;
	return oss.str();
}

template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
std::string toStrVariadic(const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g, const H& h, const I& i) {
	std::stringstream oss;
	oss << a << b << c << d << e << f << g << h << i;
	return oss.str();
}
#else
template<typename T>
void addVarSS(std::stringstream& oss,T value) {
	oss << value;
}

template<typename T, typename... Args>
void addVarSS(std::stringstream& oss,T value, Args... args) {
	oss << value;
	addVarSS(oss, args...);
}

template<typename... Args>
std::string toStrVariadic(Args... args) {
	std::stringstream oss;
	addVarSS(oss, args...);
	return oss.str();
}
#endif

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
