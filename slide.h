#ifndef SLIDE_H
#define SLIDE_H

#include <list>
#include "common.h"

class Thing;

class Slide {
public:
	shared_ptr<Slide> prevSlide;
	shared_ptr<Slide> nextSlide;

	std::list<shared_ptr<Thing>> thingList;
	std::list<string> notes;

private:
	string name;

public:
	void AttachThing(shared_ptr<Thing>);
	void AddNotes(const string&);
	void SetName(const string&);

	void _Enter();
	void _Exit();
	void _Render(Window);
	void _Update(double);

	virtual void Enter() {}
	virtual void Exit() {}
	virtual void Render(Window) {}
	virtual void Update(double) {}
};

class TitleSlide : public Slide {
public:
	sf::Text title;
	sf::Text subtitle;

public:
	TitleSlide(string, string = "");

	void Render(Window) override;
};

class ContentSlide : public Slide {
public:
	sf::Text title;

public:
	ContentSlide(string);

	void Render(Window) override;
};

#endif