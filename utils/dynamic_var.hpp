#ifndef DYNAMIC_VAR_HPP
#define DYNAMIC_VAR_HPP

namespace cpplugin {
	template<typename T>
	class DynamicVar {
		using T_ptr = T*;
	public:
		DynamicVar() : _var{new T()} {}
		~DynamicVar() { delete _var; }
		operator T_ptr() { return _var; }
	private:
		T_ptr _var;
	};
}

#endif