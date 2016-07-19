#ifndef CPPLUGIN_EXPORT_PLUGIN_HPP
#define CPPLUGIN_EXPORT_PLUGIN_HPP

	#ifndef CPPLUGIN_MAIN
		#include "utils/version.hpp"
		#include "plugin_metabase.hpp"
		#include "def_macros.hpp"

		#include <type_traits>
		using std::is_class;
		using std::is_base_of;

		#define REGISTER_PLUGIN(P) 							\
			static_assert(is_class<P>::value, "You have to register the plugin class.");\
			static_assert(is_base_of<cpplugin::PluginMetabase, P>::value, "The registred plugin MUST be a child of cpplugin::PluginBase.");\
			void * CPPLUGIN_CONSTRUCTOR() {					\
				return reinterpret_cast<void *>(new P());	\
			}												\
			void CPPLUGIN_DESTRUCTOR(void * plugin) {		\
				delete reinterpret_cast<P *>(plugin);		\
			}												\
			const char * CPPLUGIN_INTERFACE_NAME() {		\
				return apiName();							\
			}												\
			const cpplugin::Version CPPLUGIN_VERSION() {	\
				return apiVersion();						\
			}


		extern "C" {
			void * CPPLUGIN_DYNAMIC_FUNCTION CPPLUGIN_CONSTRUCTOR();
			void CPPLUGIN_DYNAMIC_FUNCTION CPPLUGIN_DESTRUCTOR(void * plugin);
			const char * CPPLUGIN_DYNAMIC_FUNCTION CPPLUGIN_INTERFACE_NAME();
			const cpplugin::Version CPPLUGIN_DYNAMIC_FUNCTION CPPLUGIN_VERSION();
		}

	#else
		#define REGISTER_PLUGIN(P)
	#endif

#endif