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

}

ErrorLogging::~ErrorLogging() {

}

ErrorLogging* ErrorLogging::getInstance() {
    if (!instance) instance = new ErrorLogging();
    return instance;
}

void ErrorLogging::log(LogLevel level, const std::string& logPrefix, const std::string& text) {
    switch(level) {
    case LogLevel::LOGGING:
        std::cout << "LOG:" << logPrefix << text << std::endl;
        break;
    case LogLevel::WARNING:
        std::cout << "WARN:" << logPrefix << text << std::endl;
        break;
    case LogLevel::ERROR:
        std::cerr << "ERR:" << logPrefix << text << std::endl;
        break;
    }
}
