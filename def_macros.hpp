#ifndef CPPLUGIN_DYNAMIC_FUNCTION_UTILS_HPP
#define CPPLUGIN_DYNAMIC_FUNCTION_UTILS_HPP

	#define CPPLUGIN_CONSTRUCTOR cpplugin_constructor
	#define CPPLUGIN_DESTRUCTOR cpplugin_destructor
	#define CPPLUGIN_INTERFACE_NAME cpplugin_getInterfaceName
	#define CPPLUGIN_VERSION cpplugin_getVersion


	#ifdef _WIN32
		//Windows

		#include <Windows.h>
		namespace cpplugin {
			using Path = LPCTSTR;
		}

		#define CPPLUGIN_LOAD_LIB(path) LoadLibrary(path)
		#define CPPLUGIN_FREE_LIB(lib) FreeLibrary(lib)
		#define CPPLUGIN__GET_DYNAMIC_FUNCTION(lib, func) func = reinterpret_cast<decltype(func)>(GetProcAddress(lib, #func))
		#define CPPLUGIN_GET_DYNAMIC_FUNCTION(lib, func) _CPPLUGIN_GET_DYNAMIC_FUNCTION(lib, func)

		#ifndef CPPLUGIN_MAIN
			#define CPPLUGIN_DYNAMIC_FUNCTION __declspec(dllexport) __stdcall
		#else
			#define CPPLUGIN_DYNAMIC_FUNCTION __stdcall
		#endif


	#else
		//Linux and Mac OS X

		#include <dlfcn.h>
		namespace cpplugin {
			char const *;
		}

		#define CPPLUGIN_LOAD_LIB(path) dlopen(path, RTLD_LAZY)
		#define CPPLUGIN_FREE_LIB(lib) dlclose(lib)
		#define _CPPLUGIN_GET_DYNAMIC_FUNCTION(lib, func) func = reinterpret_cast<decltype(func)>(dlsym(lib, #func))
		#define CPPLUGIN_GET_DYNAMIC_FUNCTION(lib, func) _CPPLUGIN_GET_DYNAMIC_FUNCTION(lib, func)

		#define CPPLUGIN_DYNAMIC_FUNCTION


	#endif

#endif