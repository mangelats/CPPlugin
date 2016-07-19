#ifndef CPPLUGIN_DYNAMIC_PLUGIN_LOADER_HPP
#define CPPLUGIN_DYNAMIC_PLUGIN_LOADER_HPP

#include "interface_base.hpp"
#include "plugin_base.hpp"
#include "def_macros.hpp"

namespace cpplugin {
	template<class Interface, class Plugin>
	class DynamicPluginLoader {
		static_assert(is_base_of<InterfaceBase, Interface>::value, "Interface must be a child of BaseInterface.");
		static_assert(is_base_of<PluginBase<Interface>, Plugin>::value, "Plugin must be a child of BasePlugin.");
	public:
		DynamicPluginLoader(Path, Interface *);
		~DynamicPluginLoader();

		bool isLoaded() const;
		bool isVersionCompatible() const;

		void reload(Interface *);
		Plugin * plugin();
	private:
		Path _path;
		Plugin * _plugin;
		void * (CPPLUGIN_DYNAMIC_FUNCTION * CPPLUGIN_CONSTRUCTOR)();
		void (CPPLUGIN_DYNAMIC_FUNCTION * CPPLUGIN_DESTRUCTOR)(void * plugin);
		const char * (CPPLUGIN_DYNAMIC_FUNCTION * CPPLUGIN_INTERFACE_NAME)();
		const Version (CPPLUGIN_DYNAMIC_FUNCTION * CPPLUGIN_VERSION)();


		#ifdef _WIN32
			HMODULE _library;
		#else
			void * _library;
		#endif
	};


	template<class Interface, class Plugin>
	DynamicPluginLoader<Interface, Plugin>::DynamicPluginLoader(Path path, Interface * i) : 
	_path{path}, _plugin{nullptr},
	CPPLUGIN_CONSTRUCTOR{nullptr}, CPPLUGIN_DESTRUCTOR{nullptr}, CPPLUGIN_INTERFACE_NAME{nullptr}, CPPLUGIN_VERSION{nullptr} {
		_library = CPPLUGIN_LOAD_LIB(_path);
		if (_library) {
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_CONSTRUCTOR);
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_DESTRUCTOR);
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_INTERFACE_NAME);
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_VERSION);

			if (isVersionCompatible()) {
				_plugin = reinterpret_cast<Plugin *>(CPPLUGIN_CONSTRUCTOR());
				_plugin->loaded(i);
			}
		}
	}

	template<class Interface, class Plugin>
	DynamicPluginLoader<Interface, Plugin>::~DynamicPluginLoader() {
		if (_plugin != nullptr) {
			_plugin->unloaded();
			CPPLUGIN_DESTRUCTOR(_plugin);
		}
		if (_library != nullptr)
			CPPLUGIN_FREE_LIB(_library);
	}

	template<class Interface, class Plugin>
	bool DynamicPluginLoader<Interface, Plugin>::isLoaded() const {
		return _plugin != nullptr;
	}

	template<class Interface, class Plugin>
	bool DynamicPluginLoader<Interface, Plugin>::isVersionCompatible() const {
		if (CPPLUGIN_VERSION == nullptr || CPPLUGIN_INTERFACE_NAME == nullptr)
			return false;
		
		Version iv = apiVersion();
		Version pv = CPPLUGIN_VERSION();

		return iv.major() == pv.major() && iv.minor() >= pv.minor() && strcmp(apiName(), CPPLUGIN_INTERFACE_NAME()) == 0;
	}

	template<class Interface, class Plugin>
	void DynamicPluginLoader<Interface, Plugin>::reload(Interface * i) {
		if (_plugin != nullptr) {
			_plugin->unloaded();
			CPPLUGIN_DESTRUCTOR(_plugin);
			_plugin = nullptr;
		}
		
		if (_library != nullptr)
			CPPLUGIN_FREE_LIB(_library);

		CPPLUGIN_CONSTRUCTOR = nullptr;
		CPPLUGIN_DESTRUCTOR = nullptr;
		CPPLUGIN_INTERFACE_NAME = nullptr;
		CPPLUGIN_VERSION = nullptr;

		_library = CPPLUGIN_LOAD_LIB(_path);
		if (_library) {
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_CONSTRUCTOR);
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_DESTRUCTOR);
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_INTERFACE_NAME);
			CPPLUGIN_GET_DYNAMIC_FUNCTION(_library, CPPLUGIN_VERSION);

			if (isVersionCompatible()) {
				_plugin = reinterpret_cast<Plugin *>(CPPLUGIN_CONSTRUCTOR());
				_plugin->loaded(i);
			}
		}

	}

	template<class Interface, class Plugin>
	Plugin * DynamicPluginLoader<Interface, Plugin>::plugin() {
		return _plugin;
	}
}

#endif