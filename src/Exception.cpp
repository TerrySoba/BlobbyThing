/*
 * Exception.cpp
 *
 *  Created on: 05.01.2013
 *      Author: yoshi252
 */

#include "Exception.h"

namespace blobby {

/**
 * Constructor
 *
 * \param [in] message The message of the exception. Will be returned by what().
 */
Exception::Exception(const std::string& message) :
    m_message(message)
{
}

/**
 * Destructor
 */
Exception::~Exception() throw() {
}

/**
 * @return The message of the exception.
 */
const char* Exception::what() const throw() {
    return m_message.c_str();
}

} // namespace blobby
