#ifndef THING_PLAYER_H
#define THING_PLAYER_H

#include "thing.h"

class Player : public Thing{
public:
	enum Dir {
		Left = -1,
		Right = 1,
	};

	static sf::Color colors[];

public:
	float ground;

	sf::RectangleShape sprite;
	sf::Vector2f vel;
	Dir dir;

	int colorIdx;
	float colorCountdown;

	bool flashing;

public:
	Player();

	void Enter() override;
	void Update(double) override;
	void Render(Window) override;
};

#endif