#ifndef LIBRARY_MACROS_HPP
#define LIBRARY_MACROS_HPP



#ifdef __WIN32
	//Windows

	#include <Windows.h>
	using Path = LPCTSTR;

	#define LOAD_LIB(path) LoadLibrary(path)
	#define FREE_LIB(lib) FreeLibrary(lib)
	#define _GET_DYNAMIC_FUNCTION(lib, func) func = reinterpret_cast<decltype(func)>(GetProcAddress(lib, #func))

	#ifndef CPPLUGIN_INTERFACE
		#define DYNAMIC_FUNCTION __declspec(dllexport) __stdcall
	#else
		#define DYNAMIC_FUNCTION __stdcall
	#endif


#else
	//Linux and Mac OS X

	#include <dlfcn.h>
	using Path = char const *;

	#define LOAD_LIB(path) dlopen(path, RTLD_LAZY)
	#define FREE_LIB(lib) dlclose(lib)
	#define _GET_DYNAMIC_FUNCTION(lib, func) func = reinterpret_cast<decltype(func)>(dlsym(lib, #func))

	#define DYNAMIC_FUNCTION


#endif


#define GET_DYNAMIC_FUNCTION(lib, func) _GET_DYNAMIC_FUNCTION(lib, func)



#endif