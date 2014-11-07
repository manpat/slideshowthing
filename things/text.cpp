#include "things/text.h"
#include "main.h"

Text::Text(string _s){
	text.setFont(Main::font);
	text.setString(_s);
}

void Text::Render(Window w){
	w->draw(text);
}

void Text::SetPosition(sf::Vector2f _pos){
	text.setPosition(_pos);
}

void Text::SetJustify(Justify _jus){
	auto r = text.getLocalBounds();
	switch(_jus){
		case Text::Left:{
			text.setOrigin(0.f, r.height);
			break;
		}
		case Text::Center:{
			text.setOrigin(r.width/2.f, r.height);
			break;
		}
		case Text::Right:{
			text.setOrigin(r.width, r.height);
			break;
		}
	}
}