#ifndef THING_H
#define THING_H

#include "common.h"

class Thing{
public:
	virtual void Enter() {}
	virtual void Exit() {}
	virtual void Render(Window) {}
	virtual void Update(double) {}
};

#endif