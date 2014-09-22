#ifndef _DECODE_UTF8_INCLUDED
#define _DECODE_UTF8_INCLUDED

#include <vector>
#include <cstdint>


std::vector<uint32_t> decodeUtf8(const std::string& utf8);


#endif
