#pragma once

#include <string>

#include <tdme/utils/fwd-tdme.h>

using std::string;

/**
 * Run time type information utility class
 * @author Andreas Drewke
 */
class tdme::utils::RTTI {
public:
	/**
	 * @brief Demangle c++ symbol names
	 * @return demangled name
	 */
	static const string demangle(const char* name);
};
