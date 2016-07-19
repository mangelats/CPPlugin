#ifndef _PLUGIN_BASE_HPP
#define _PLUGIN_BASE_HPP

#include "interface_base.hpp"
#include <type_traits>
using std::enable_if;
using std::is_base_of;

namespace cpplugin {
	template<class I, typename = enable_if<is_base_of<InterfaceBase, I>::value> >
	class PluginBase {
	public:
		using Interface = I;
		virtual ~PluginBase() {}
		virtual void load(Interface *) {}
		virtual void unload() {}
	};
}

#endif