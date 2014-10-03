#include <boost/format.hpp>

#include <string>
#include <sstream>


namespace blobby {
namespace string {


template<typename T>
void addVarSS(std::stringstream& oss, const T& value) {
    oss << value;
}

template<typename T, typename... Args>
void addVarSS(std::stringstream& oss, const T& value, Args... args) {
    oss << value;
    addVarSS(oss, args...);
}

template<typename... Args>
std::string join(Args... args) {
    std::stringstream oss;
    addVarSS(oss, args...);
    return oss.str();
}

template<typename T>
void addVarFmt(boost::format& f, const T& value) {
    f % value;
}

template<typename T, typename... Args>
void addVarFmt(boost::format& f, const T& value, Args... args) {
    f % value;
    addVarFmt(f, args...);
}

template<typename... Args>
std::string format(const std::string& fmt, Args... args) {
    boost::format f(fmt);
    addVarFmt(f, args...);
    return f.str();
}


} // namespace string
} // namespace blobby
