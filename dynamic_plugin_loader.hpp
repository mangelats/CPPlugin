#ifndef DYNAMIC_PLUGIN_LOADER_HPP
#define DYNAMIC_PLUGIN_LOADER_HPP

#include "library_macros.hpp"
#include "interface_base.hpp"
#include "plugin_base.hpp"

#include <cstring>
#include <type_traits>
using std::is_class;
using std::is_base_of;
using std::is_abstract;

#include <iostream>
using std::cout;
using std::endl;


#define CONSTRUCTOR cpplugin_constructor
#define DESTRUCTOR cpplugin_destructor
#define INTERFACE_NAME cpplugin_getInterfaceName
#define VERSION cpplugin_getVersion


#ifndef CPPLUGIN_INTERFACE

	#define REGISTER_PLUGIN(P) 							\
		static_assert(is_class<P>::value, "You have to register the plugin class.");\
		static_assert(is_base_of<cpplugin::PluginBase<P::Interface>, P>::value, "The registred plugin MUST be a child of cpplugin::PluginBase.");\
		void * CONSTRUCTOR() {							\
			return reinterpret_cast<void *>(new P());	\
		}												\
		void DESTRUCTOR(void * plugin) {				\
			delete reinterpret_cast<P *>(plugin);		\
		}												\
		const char * INTERFACE_NAME() {					\
			return apiName();							\
		}												\
		const cpplugin::Version VERSION() {				\
			return apiVersion();						\
		}


	extern "C" {
		void * DYNAMIC_FUNCTION CONSTRUCTOR();
		void DYNAMIC_FUNCTION DESTRUCTOR(void * plugin);
		const char * DYNAMIC_FUNCTION INTERFACE_NAME();
		const cpplugin::Version DYNAMIC_FUNCTION VERSION();
	}



#else

	#define REGISTER_PLUGIN(P)



	namespace cpplugin {

		template<class Interface, class Plugin>
		class DynamicPluginLoader {
			static_assert(is_base_of<InterfaceBase, Interface>::value, "Interface must be a child of BaseInterface.");
			static_assert(is_base_of<PluginBase<Interface>, Plugin>::value, "Plugin must be a child of BasePlugin.");
		public:
			DynamicPluginLoader(Path, Interface *);
			~DynamicPluginLoader();

			bool isVersionCompatible() const;

			void reload(Interface *);
			Plugin * plugin();
		private:
			Path _path;
			Plugin * _plugin;
			void * (DYNAMIC_FUNCTION * CONSTRUCTOR)();
			void (DYNAMIC_FUNCTION * DESTRUCTOR)(void * plugin);
			const char * (DYNAMIC_FUNCTION * INTERFACE_NAME)();
			const Version (DYNAMIC_FUNCTION * VERSION)();

		// They are actually the same but whatever
		#ifdef _WIN32
			HMODULE _library;
		#else
			void * _library;
		#endif
		};


		template<class Interface, class Plugin>
		DynamicPluginLoader<Interface, Plugin>::DynamicPluginLoader(Path path, Interface * i) : 
			_path{path}, _plugin{nullptr}, CONSTRUCTOR{nullptr}, DESTRUCTOR{nullptr}, VERSION{nullptr} {
			_library = LOAD_LIB(_path);
			if (_library) {
				GET_DYNAMIC_FUNCTION(_library, CONSTRUCTOR);
				GET_DYNAMIC_FUNCTION(_library, DESTRUCTOR);
				GET_DYNAMIC_FUNCTION(_library, INTERFACE_NAME);
				GET_DYNAMIC_FUNCTION(_library, VERSION);

				if (isVersionCompatible()) {
					_plugin = reinterpret_cast<Plugin *>(CONSTRUCTOR());
					_plugin->load(i);
				}
			}
		}

		template<class Interface, class Plugin>
		DynamicPluginLoader<Interface, Plugin>::~DynamicPluginLoader() {
			if (_plugin != nullptr) {
				_plugin->unload();
				DESTRUCTOR(_plugin);
			}
			if (_library != nullptr)
				FREE_LIB(_library);
		}

		template<class Interface, class Plugin>
		bool DynamicPluginLoader<Interface, Plugin>::isVersionCompatible() const {
			Version iv = apiVersion();
			Version pv = VERSION();

			//INTERFACE_NAME();

			return iv.major() == pv.major() && iv.minor() >= pv.minor() && strcmp(apiName(), INTERFACE_NAME()) == 0;
		}

		template<class Interface, class Plugin>
		void DynamicPluginLoader<Interface, Plugin>::reload(Interface * i) {
			if (_plugin != nullptr) {
				_plugin->unload();
				DESTRUCTOR(_plugin);
				_plugin = nullptr;
			}
			
			if (_library != nullptr)
				FREE_LIB(_library);

			CONSTRUCTOR = nullptr;
			DESTRUCTOR = nullptr;
			VERSION = nullptr;

			_library = LOAD_LIB(_path);
			if (_library) {
				GET_DYNAMIC_FUNCTION(_library, CONSTRUCTOR);
				GET_DYNAMIC_FUNCTION(_library, DESTRUCTOR);
				GET_DYNAMIC_FUNCTION(_library, INTERFACE_NAME);
				GET_DYNAMIC_FUNCTION(_library, VERSION);

				if (isVersionCompatible()) {
					_plugin = reinterpret_cast<Plugin *>(CONSTRUCTOR());
					_plugin->load(i);
				}
			}

		}

		template<class Interface, class Plugin>
		Plugin * DynamicPluginLoader<Interface, Plugin>::plugin() {
			return _plugin;
		}
	}


//CPPLUGIN_PLUGIN end
#endif



// Clean up macros and defines
#undef DYNAMIC_FUNCTION
#undef LOAD_LIB
#undef FREE_LIB
#undef GET_DYNAMIC_FUNCTION
#undef _GET_DYNAMIC_FUNCTION


#endif