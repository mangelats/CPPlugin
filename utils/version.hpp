#ifndef VERSION_HPP
#define VERSION_HPP

namespace cpplugin {
	class Version {
		using uint = unsigned int;
	public:
		constexpr Version(uint major, uint minor) : _major{major}, _minor{minor} {}
		constexpr Version(uint major) : Version(major, 0) {}
		constexpr Version(const Version& b) : Version(b._major, b._minor) {}

		constexpr uint major() const { return _major; }
		constexpr uint minor() const { return _minor; }

		constexpr bool operator==(const Version b) const { return _major == b._major && _minor == b._minor; }
		constexpr bool operator!=(const Version b) const { return !(*this == b); }
		constexpr bool operator>(const Version b) const { return _major > b._major || (_major >= b._major && _minor > b._minor); }
		constexpr bool operator<(const Version b) const { return _major < b._major || (_major <= b._major && _minor < b._minor); }
		constexpr bool operator>=(const Version b) const { return !(*this < b); }
		constexpr bool operator<=(const Version b) const { return !(*this > b); }

	private:
		uint _major;
		uint _minor;
	};
}

#endif