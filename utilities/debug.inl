#ifndef DEBUG_INL
#define DEBUG_INL

#include <iostream>

template<typename T>
void Debug::Error(const T& s){
	Debug::Print(s, errorSetting);
}
template<typename T>
void Debug::Warning(const T& s){
	Debug::Print(s, warningSetting);
}

template<typename T>
void Debug::Print(const T& s, const Setting& set){
	static std::string resetStr = GetColorEscape(Debug::Reset);
	std::string escColor = GetColorEscape(set.color);

	std::cout << "[" << escColor << set.type << resetStr << "] " << s << std::endl;

	if(set.log)
		GetLog() << "[" << set.type << "] " << s << std::endl;

}

template<typename T>
Debug& Debug::operator<<(const T& s){
	Debug::Print(s, setting);

	return *this;
}

template<typename T>
Debug::Append& Debug::Append::operator<<(const T& s){
	if(!first && autospace)
		buf << " ";
	
	buf << s;

	first = false;
	return *this;
}

#endif