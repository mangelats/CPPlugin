#ifndef CPPLUGIN_PLUGIN_BASE_HPP
#define CPPLUGIN_PLUGIN_BASE_HPP

#include "plugin_metabase.hpp"
#include "interface_base.hpp"
#include "export_plugin.hpp"
#include <type_traits>
using std::is_base_of;


namespace cpplugin {
	template<class I>
	class PluginBase : public PluginMetabase {
		static_assert(is_base_of<InterfaceBase, I>::value, "The template parameter must be a child of cpplugin::InterfaceBase.");
	protected:
		using Interface_t = I;
	public:
		virtual ~PluginBase() {}
		virtual void loaded(I *) {}
		virtual void unloaded() {}
	};
}


#endif