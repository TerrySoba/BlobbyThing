#include <boost/format.hpp>

#include <string>
#include <sstream>


namespace blobby {
namespace string {

#ifdef _MSC_VER
// Microsoft Visual C++ does not support variadic templates, so we have to simulate them
template <typename A>
std::string join(const A& a) {
	std::stringstream oss;
	oss << a;
	return oss.str();
}

template <typename A, typename B>
std::string join(const A& a, const B& b) {
	std::stringstream oss;
	oss << a << b;
	return oss.str();
}

template <typename A, typename B, typename C>
std::string join(const A& a, const B& b, const C& c) {
	std::stringstream oss;
	oss << a << b << c;
	return oss.str();
}

template <typename A, typename B, typename C, typename D>
std::string join(const A& a, const B& b, const C& c, const D& d) {
	std::stringstream oss;
	oss << a << b << c << d;
	return oss.str();
}

template <typename A, typename B, typename C, typename D, typename E>
std::string join(const A& a, const B& b, const C& c, const D& d, const E& e) {
	std::stringstream oss;
	oss << a << b << c << d << e;
	return oss.str();
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
std::string join(const A& a, const B& b, const C& c, const D& d, const E& e, const F& f) {
	std::stringstream oss;
	oss << a << b << c << d << e << f;
	return oss.str();
}

template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
std::string join(const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g, const H& h, const I& i) {
	std::stringstream oss;
	oss << a << b << c << d << e << f << g << h << i;
	return oss.str();
}
#else
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
#endif

} // namespace string
} // namespace blobby
