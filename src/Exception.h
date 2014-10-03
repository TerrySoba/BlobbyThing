/*
 * Exception.cpp
 *
 *  Created on: 05.01.2013
 *      Author: yoshi252
 */

#include <exception>
#include <string>

#include "ErrorLogging.h"

#define THROW_BLOBBY_EXCEPTION(...) \
    ERR("Blobby Exception: ", __VA_ARGS__); \
    throw blobby::Exception(blobby::string::join(__VA_ARGS__))

namespace blobby {

/**
 * This is the base class of all exceptions thrown by BlobbyThing.
 *
 * To throw an exception you should use THROW_BLOBBY_EXCEPTION() as it will
 * also log an error.
 */
class Exception : public std::exception {
public:
    Exception(const std::string& message);
    virtual ~Exception() throw();
    virtual const char* what() const throw();

private:
    std::string m_message;
};

} // namespace blobby
