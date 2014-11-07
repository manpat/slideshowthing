#ifndef DEBUG_H
#define DEBUG_H

#include <fstream>
#include <string>
#include <sstream>
#include <SFML/System/String.hpp>

class Debug{
public:
	enum Color{
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Purple,
		Cyan,
		White,

		Reset
	};

	class Setting {
	public:
		Setting(const std::string _type, Color _color = Color::Reset, bool _log = true)
			: type(_type), color(_color), log(_log){}

		Setting(Color _color, bool _log = true)
			: Setting("Debug", _color, _log){}

		Setting(bool _log = true)
			: Setting("Debug", Color::Reset, _log){}

		std::string type;
		Color color;
		bool log;
	};

	class NL{};
	class Append;

	static Setting defaultSetting;
	static Setting warningSetting;
	static Setting errorSetting;
	static Setting pauseSetting;

protected:
	static std::string logName;
	static std::fstream& GetLog();
	static std::string GetColorEscape(Color);

	Setting setting; // For streams

public:
	template<typename T>
	static void Warning(const T&);
	template<typename T>
	static void Error(const T&);
	
	template<typename T>
	static void Print(const T&, const Setting& = defaultSetting);
	static void Print(const Append&, const Setting& = defaultSetting);
	static void Print(const bool&, const Setting& = defaultSetting);
	static void Print();

	template<typename T>
	Debug& operator<<(const T&);
	Debug& operator<<(const Setting&);
	Debug& operator<<(const NL&);

	static void Sleep(float);
	static void Pause();
};

class Debug::Append{
protected:
	std::stringstream buf;
	bool first;
	bool autospace;

public:
	Append(bool = true);

	template<typename T>
	Append& operator<<(const T&);

	operator std::string() const;
	operator sf::String() const;
};

#include "debug.inl"

#endif