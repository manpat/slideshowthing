#include "slidemanager.h"
#include "slide.h"
#include "thing.h"

SlideManager::SlideManager(){
	firstSlide = lastSlide = currentSlide = nullptr;
}

void SlideManager::AddSlide(shared_ptr<Slide> slide){
	if(!firstSlide){
		firstSlide = slide;
		lastSlide = slide;
		currentSlide = slide;
		_Enter();
	}else{
		lastSlide->nextSlide = slide;
		slide->prevSlide = lastSlide;
		lastSlide = slide;
	}
}

void SlideManager::AddPersistent(shared_ptr<Thing> thing){
	persistentThings.push_back(thing);
}

void SlideManager::NextSlide(){
	if(!currentSlide) return;

	if(auto s = currentSlide->nextSlide){
		_Exit();
		currentSlide = s;
		_Enter();
	}
}
void SlideManager::PrevSlide(){
	if(!currentSlide) return;

	if(auto s = currentSlide->prevSlide){
		_Exit();
		currentSlide = s;
		_Enter();
	}
}

shared_ptr<Slide> SlideManager::GetSlide(){
	return currentSlide;
}

void SlideManager::Update(double dt){
	if(currentSlide)
		currentSlide->_Update(dt);

	for(auto& thing : persistentThings){
		thing->Update(dt);
	}
}

void SlideManager::Render(Window w){
	if(currentSlide)
		currentSlide->_Render(w);

	for(auto& thing : persistentThings){
		thing->Render(w);
	}
}

void SlideManager::_Enter(){
	if(!currentSlide) return;

	currentSlide->_Enter();

	for(auto& thing : persistentThings){
		thing->Enter();
	}	
}
void SlideManager::_Exit(){
	if(!currentSlide) return;
	
	currentSlide->_Exit();

	for(auto& thing : persistentThings){
		thing->Exit();
	}	
}