#ifndef SLIDEMANAGER_H
#define SLIDEMANAGER_H

#include <list>
#include "common.h"

class Slide;
class Thing;

class SlideManager {
public:
	shared_ptr<Slide> firstSlide;
	shared_ptr<Slide> lastSlide;
	shared_ptr<Slide> currentSlide;

	std::list<shared_ptr<Thing>> persistentThings;

private:
	void _Enter();
	void _Exit();

public:
	SlideManager();

	void AddSlide(shared_ptr<Slide>);
	void AddPersistent(shared_ptr<Thing>);
	void NextSlide();
	void PrevSlide();
	shared_ptr<Slide> GetSlide();

	void Update(double);
	void Render(Window);
};

#endif