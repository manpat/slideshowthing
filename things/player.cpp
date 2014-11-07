#include "things/player.h"
#include "slidemanager.h"
#include "main.h"

sf::Color Player::colors[] = {
	sf::Color(255, 255, 0),
	sf::Color(255, 0, 255),
	sf::Color(0, 255, 255),
};

Player::Player(){
	// Load sprite
	auto size = sf::Vector2f(50.f, 50.f);
	sprite.setSize(size);
	sprite.setOrigin(size/2.f);

	ground = Main::GetHalf().y * 3.f / 2.f;
	dir = Right;

	// tex.loadFromFile("images/guy.png");
	// sprite.setTexture(tex);
	// sprite.setTextureRect(sf::IntRect(0, 24, 16, 24));
	// sprite.setScale(sf::Vector2f(5.f, 5.f));
	colorCountdown = 0.1f;
	colorIdx = 0;
	flashing = false;
}

void Player::Update(double dt){
	auto pos = sprite.getPosition();
	auto sSize = Main::GetSize();
	
	vel.x = 0.f;

	vel.x += (float) sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	vel.x -= (float) sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	vel.x *= 300.f;
	vel.x *= dt;

	if(pos.y > ground) {
		pos.y = ground;
		vel.y = 0.f;

	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		vel.y = -500.f * dt;
	}else{
		vel.y += 10.f * dt;
	}

	pos += vel;

	if(abs(vel.x) > 0.f){
		dir = (Dir)(vel.x / abs(vel.x));
	}

	sprite.setPosition(pos);

	if(pos.x > sSize.x) Main::slideManager->NextSlide();
	else if(pos.x < 0) Main::slideManager->PrevSlide();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) flashing = true;

	colorCountdown -= dt;
	if(colorCountdown <= 0.f && flashing){
		colorCountdown = 1.f/20.f;

		sprite.setFillColor(colors[colorIdx]);
		colorIdx = (colorIdx+1)%3;
	}
}

void Player::Render(Window w){
	w->draw(sprite);
}

void Player::Enter(){
	auto size = sf::Vector2f(50.f, 50.f);
	auto sSize = Main::GetSize();

	auto pos = sprite.getPosition();
	// pos.x = size.x + 10.f;

	switch(dir){
		case Left:
			pos.x = sSize.x - size.x - 10.f;
			break;

		case Right:
			pos.x = size.x + 10.f;
			break;

		default:
			Debug() << "FUCK" << dir;
	}

	sprite.setPosition(pos);

	flashing = false;
	sprite.setFillColor(sf::Color::White);
}