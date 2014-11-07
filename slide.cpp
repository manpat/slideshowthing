#include "slide.h"
#include "main.h"
#include "thing.h"

///////////////////////////////////////////////////////////////////
///// Main Slide //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void Slide::_Enter(){
	Enter();

	for(auto& t : thingList){
		t->Enter();
	}

	Debug::Setting nameSett("Slide", Debug::Green);
	Debug::Setting noteSett("Note", Debug::Blue);

	Debug() << nameSett << name;

	for(auto& n : notes){
		Debug() << noteSett << n;
	}
}
void Slide::_Exit(){
	Exit();

	for(auto& t : thingList){
		t->Exit();
	}
}
void Slide::_Render(Window w){
	Render(w);

	for(auto& t : thingList){
		t->Render(w);
	}
}
void Slide::_Update(double dt){
	Update(dt);

	for(auto& t : thingList){
		t->Update(dt);
	}
}

void Slide::AttachThing(shared_ptr<Thing> t){
	thingList.push_back(t);
}

void Slide::AddNotes(const string& n){
	notes.push_back(n);
}
void Slide::SetName(const string& n){
	name = n;
}

///////////////////////////////////////////////////////////////////
///// TitleSlide //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

TitleSlide::TitleSlide(string _title, string _subtitle){
	title.setString(_title);
	subtitle.setString(_subtitle);

	title.setFont(Main::font);
	subtitle.setFont(Main::font);

	title.setCharacterSize(100);
	subtitle.setCharacterSize(40);

	auto r = title.getLocalBounds();
	title.setOrigin(r.width/2, r.height);
	auto r2 = subtitle.getLocalBounds();
	subtitle.setOrigin(r2.width/2, r2.height);

	title.setPosition(Main::GetHalf());
	subtitle.setPosition(Main::GetHalf() + sf::Vector2f(0, r.height + r2.height*2));
}

void TitleSlide::Render(Window w){
	w->draw(title);
	w->draw(subtitle);
}

///////////////////////////////////////////////////////////////////
///// Content Slide ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

ContentSlide::ContentSlide(string _title){
	title.setString(_title);
	title.setFont(Main::font);

	title.setCharacterSize(100);

	auto r = title.getLocalBounds();
	title.setOrigin(r.width/2, r.height);

	auto pos = Main::GetHalf();
	pos.y = r.height;
	title.setPosition(pos);
}

void ContentSlide::Render(Window w){
	w->draw(title);
}