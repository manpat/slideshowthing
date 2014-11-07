#ifndef MAIN_H
#define MAIN_H

#include "common.h"

class SlideManager;

class Main {
public:
	static Window window;
	static shared_ptr<SlideManager> slideManager;

	static sf::Clock clock;
	static double dt;
	static double timeRunning;

	static sf::Font font;

public:
	static void Init();
	static void Run();

	static sf::Vector2f GetSize();
	static sf::Vector2f GetHalf();
};

#endif