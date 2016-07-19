#ifndef CPPLUGIN_INTERFACE_BASE_HPP
#define CPPLUGIN_INTERFACE_BASE_HPP

#include "utils/version.hpp"

namespace cpplugin {
	class InterfaceBase {
	public:
		virtual ~InterfaceBase() {}
	};
}

constexpr char const * apiName();
constexpr cpplugin::Version apiVersion();

#endif