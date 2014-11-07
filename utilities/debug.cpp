#ifndef DEBUG_CPP
#define DEBUG_CPP

#include "debug.h"
#include <thread>
#include <chrono>
#include <cmath>
#include <climits>

using std::fstream;
using std::string;

string Debug::logName = "default.log";
Debug::Setting Debug::defaultSetting = Debug::Setting();
Debug::Setting Debug::warningSetting = Debug::Setting("Warning", Debug::Yellow);
Debug::Setting Debug::errorSetting   = Debug::Setting("Error", Debug::Red);
Debug::Setting Debug::pauseSetting   = Debug::Setting("Pause", Debug::Purple, false);

fstream& Debug::GetLog(){
	static fstream $log;

	if(!$log.is_open()){
		$log.open(logName, std::ios_base::out);
	}

	return $log;
}

string Debug::GetColorEscape(Debug::Color c){
	string r = "0";

	switch(c){
		case Debug::Black:
			r = "30;1";
			break;
		case Debug::Red:
			r = "31;1";
			break;
		case Debug::Green:
			r = "32;1";
			break;
		case Debug::Yellow:
			r = "33;1";
			break;
		case Debug::Blue:
			r = "34;1";
			break;
		case Debug::Purple:
			r = "35;1";
			break;
		case Debug::Cyan:
			r = "36;1";
			break;
		case Debug::White:
			r = "37;1";
			break;

		case Debug::Reset:
		default:
			break;
	}	

	return "\e["+r+"m";
}

void Debug::Print(){
	std::cout << std::endl;
	GetLog() << std::endl;
}

void Debug::Print(const Append& a, const Setting& set){
	Debug::Print((string)a, set);
}
void Debug::Print(const bool& a, const Setting& set){
	Debug::Print(a?"true":"false", set);
}

void Debug::Sleep(float ms){
	std::chrono::microseconds timeout(static_cast<int>(floor(ms*1000.f)));
	std::this_thread::sleep_for( timeout );
}

void Debug::Pause(){
	using std::cin;

	Debug::Print("Press any key to continue...", Debug::pauseSetting);
	cin.sync();
	cin.clear();
	// if(cin.gcount() > 0) cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Debug& Debug::operator<<(const Debug::Setting& s){
	setting = s;
	return *this;
}
Debug& Debug::operator<<(const Debug::NL& s){
	Debug::Print();
	return *this;
}

Debug::Append::Append(bool _autospace){
	first = true;
	autospace = _autospace;
}

Debug::Append::operator string() const{
	return buf.str();
}


Debug::Append::operator sf::String() const{
	return buf.str();
}

#endif