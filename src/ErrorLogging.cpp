/*
 * ErrorLogging.cpp
 *
 *  Created on: 05.02.2012
 *      Author: yoshi252
 */

#include "ErrorLogging.h"

#include <stdio.h>

std::shared_ptr<ErrorLogging> ErrorLogging::m_instance;

ErrorLogging::ErrorLogging() {

}

ErrorLogging::~ErrorLogging() {

}

std::shared_ptr<ErrorLogging> ErrorLogging::getInstance() {
    if (!m_instance) m_instance.reset(new ErrorLogging);
    return m_instance;
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
