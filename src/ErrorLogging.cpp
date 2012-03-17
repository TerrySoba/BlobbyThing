/*
 * ErrorLogging.cpp
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#include "ErrorLogging.h"

#include <stdio.h>

ErrorLogging* ErrorLogging::instance = 0;

ErrorLogging::ErrorLogging() {
	// TODO Auto-generated constructor stub

}

ErrorLogging::~ErrorLogging() {
	// TODO Auto-generated destructor stub
}

ErrorLogging* ErrorLogging::getInstance() {
	if (!instance) instance = new ErrorLogging();
	return instance;
}

void ErrorLogging::log(LogLevel level, std::string logPrefix, std::string text) {
	switch(level) {
	case LOGGING:
		std::cout << "LOG:" << logPrefix << text << std::endl;
		break;
	case WARNING:
		std::cout << "WARN:" << logPrefix << text << std::endl;
		break;
	case ERROR:
		std::cerr << "ERR:" << logPrefix << text << std::endl;
		break;
	}
}
