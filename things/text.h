#ifndef THING_TEXT_H
#define THING_TEXT_H

#include "thing.h"

class Text : public Thing {
public:
	enum Justify {
		Left, Center, Right
	};

	sf::Text text;
	Justify justify;

public:
	Text(string);

	void SetPosition(sf::Vector2f);
	void SetJustify(Justify);

	void Render(Window) override;
};

#endif