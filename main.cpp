#include "common.h"
#include "main.h"

#include "slidemanager.h"
#include "slide.h"

#include "things.h"

Window Main::window = nullptr;
shared_ptr<SlideManager> Main::slideManager = nullptr;

sf::Clock Main::clock = sf::Clock();
double Main::dt = 0.0;
double Main::timeRunning = 0.0;

sf::Font Main::font = sf::Font();

class Ground : public Thing {
	sf::RectangleShape shape;
public:
	Ground(){
		shape.setSize(Main::GetSize());

		auto p = sf::Vector2f(0.f, 0.f);
		p.y = Main::GetHalf().y * 3.f / 2.f + 25.f;

		shape.setPosition(p);

		float g = 0.12f * 255;
		shape.setFillColor(sf::Color(g,g,g));
	}

	void Render(Window w) override {
		w->draw(shape);
	}
};

void Main::Init(){
	if(!font.loadFromFile("arial.ttf")) throw("Font load failed");

	auto vm = sf::VideoMode::getFullscreenModes();
	Debug() << (Debug::Append()
			<< vm[0].width << vm[0].height << vm[0].bitsPerPixel);

	window = make_shared<sf::RenderWindow>(vm[0], "Things");
	slideManager = make_shared<SlideManager>();

	slideManager->AddPersistent(make_shared<Ground>());

	{
		auto s = make_shared<TitleSlide>("Hello");

		s->SetName("Hello");

		slideManager->AddSlide(s);
	}

	// aboot me
	// 	-- I love programming
	//	-- Why?
	//		-- Math, Phyiscs
	//		-- Predictable, most of the time
	{
		auto s = make_shared<TitleSlide>("Who the hell is this guy?");

		s->SetName("Who the fuck am I?");
		s->AddNotes("Name");
		s->AddNotes("Programmer");
		s->AddNotes("Programming fo' years");
		s->AddNotes("Love it, everyone should do it");
		s->AddNotes("Why?");
		s->AddNotes("\t\tMaths, Physics");
		s->AddNotes("\t\tPredictability (most of the time)");

		slideManager->AddSlide(s);
	}

	// WTF am I talking aboot?
	{
		auto s = make_shared<TitleSlide>("Why am I here?");

		s->SetName("WTF am I talking aboot?");
		s->AddNotes("Women in games programming, idiot");
		s->AddNotes("Their scarcity in the industry");
		s->AddNotes("Why that might be");
		s->AddNotes("What's being done about it");
		s->AddNotes("What can be done about it");

		slideManager->AddSlide(s);
	}

	// People like myself exist in all genders
	//	-- I've met both males and females that share the same appreciation for these things

	//	-- Only ever met guy programmers
	//	-- Hard pressed to say I've met any girl programmers
	//	-- None in classes/tutorials/studio
	{
		auto s = make_shared<TitleSlide>("Where are they?");

		s->SetName("Where are they?");
		s->AddNotes("Met males + females, like math/physics");
		s->AddNotes("Met male programmers");
		s->AddNotes("Can't say met girl programmers");
		s->AddNotes("None in studio/programming tutorials");

		slideManager->AddSlide(s);
	}

	// What gives?
	//	-- Certainly aren't mythical, stole the idea to make a game out of a presentation
	//		from Adriel, the female game dev whom organised the train jam.

	//	-- Certainly isn't a freak of probability. 3% of games programmers are female.
	{
		auto s = make_shared<TitleSlide>("What gives?");

		s->SetName("What gives?");
		s->AddNotes("Female game programmers not mythical");
		s->AddNotes("Stole presentation idea from Adriel Wallick");
		s->AddNotes("\t\ttrain jam");

		s->AddNotes("They've been there since cs's infancy");

		s->AddNotes("Ada lovelace");
		s->AddNotes("\t\tAttributed with first program");
		s->AddNotes("\t\tBefore machine built");
		s->AddNotes("\t\tWasn't built");

		s->AddNotes("Eniac programmed by 6 women");
		s->AddNotes("\t\tProgrammed before it was a thing");
		s->AddNotes("\t\tProgrammed in logical diagrams, switches, and cables");

		s->AddNotes("Grace Hopper");
		s->AddNotes("\t\tInvented the first compiler");
		s->AddNotes("\t\tCalled mother of COBOL");

		s->AddNotes("Why haven't I met any");
		s->AddNotes("Despite cs roots, as of 2011 less than 12% cs batchelors awarded to females");
		s->AddNotes("As of 2011 only 3% of gp'ers are female");
		s->AddNotes("Given class size, probability says no");

		slideManager->AddSlide(s);
	}

	// Why Tho?
	//	-- Wage gap
	//	-- Harassment
	//	-- Few role models
	//		-- Chicken and egg
	//		-- Linux and games
	//	-- Social expectations
	//	-- Generally shitty environment	
	{
		auto s = make_shared<TitleSlide>("But why?");

		s->SetName("Why tho?");
		s->AddNotes("Wage gap");
		s->AddNotes("\t\t$10,000 difference");

		s->AddNotes("Few modern role models");
		s->AddNotes("\t\tChicken+egg");
		s->AddNotes("\t\tLinux+games");

		s->AddNotes("Social expectations");
		s->AddNotes("\t\tThink stereotypical programmer/IT person");
		s->AddNotes("\t\tWhat gender immediately comes to mind?");
		s->AddNotes("\t\tSocial expectations suck");

		s->AddNotes("Harassment");
		s->AddNotes("\t\tGamergate");
		s->AddNotes("\t\tStarted as revenge porn thing, zoe quinn's ex");
		s->AddNotes("\t\t4chan and things went to shit");
		s->AddNotes("\t\tMany female devs/journalists, as well as male targeted");
		s->AddNotes("\t\tDoxxing");
		s->AddNotes("\t\tPolice called, Some forced to leave homes");
		s->AddNotes("\t\tDeath threats, Bomb threats, School shooting");
		s->AddNotes("\t\tYou know you're about ethics in game journalism when\n\t\tpeople are forced to leave their homes in fear of lives");

		slideManager->AddSlide(s);
	}

	// What is being done?
	//	-- Scholarships for women
	//	-- Programs aimed at getting girls (back) into computer science
	{
		auto s = make_shared<TitleSlide>("What is being done?");

		s->SetName("What is being done?");
		s->AddNotes("A number of institutions offer scholarships for:");
		s->AddNotes("\t\tGames dev");
		s->AddNotes("\t\tComputer science");

		s->AddNotes("Sony + Microsoft offer scholarships");

		s->AddNotes("A few groups dedicated to getting women into games");
		s->AddNotes("\t\tIGDA Women in Games Special Interest Group");
		s->AddNotes("\t\tWomen in Games Jobs");

		slideManager->AddSlide(s);
	}

	// What can be done?
	//	-- "Please program"
	//	-- Encourage friends to program
	{
		auto s = make_shared<TitleSlide>("What can be done?");

		s->SetName("What can be done?");
		s->AddNotes("\"Please program\"");
		s->AddNotes("\t\tEven if you never make money from it");
		s->AddNotes("\t\tIt'll be useful somehow");
		s->AddNotes("\t\tPlugins for photoshop, Organise music library, etc");
		s->AddNotes("Encourage friends to program");
		s->AddNotes("\t\tGet them to make plugins for photoshop, Organise music library, etc");

		s->AddNotes("Don't be a dick");

		slideManager->AddSlide(s);
	}

	auto s = make_shared<TitleSlide>("fin");
	s->SetName("Tack så mycket för din tid");
	s->AddNotes("Questions");
	slideManager->AddSlide(s);
}

void Main::Run(){
	static bool playerSpawned = false;

	while(window->isOpen()){
		sf::Event event;
		while(window->pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window->close();
			}else if(event.type == sf::Event::KeyPressed){
				auto key = event.key.code;
				if(key == sf::Keyboard::Escape){
					window->close();
				}else if(key == sf::Keyboard::Right){
					slideManager->NextSlide();

				}else if(key == sf::Keyboard::Left){
					slideManager->PrevSlide();

				}else if(key == sf::Keyboard::P && !playerSpawned){
					slideManager->AddPersistent(make_shared<Player>());
					playerSpawned = true;
				}

			}
		}

		dt = clock.restart().asSeconds();
		timeRunning += dt;

		slideManager->Update(dt);

		window->clear(sf::Color(0.1*255, 0.1*255, 0.1*255));
		slideManager->Render(window);
		window->display();
	}
}

sf::Vector2f Main::GetSize(){
	return window->getView().getSize();
}
sf::Vector2f Main::GetHalf(){
	return window->getView().getSize()/2.f;
}

int main(){
	try{
		Main::Init();
		Main::Run();
	}catch(...){
		Debug::Error("Unhandled error!");
		Debug::Pause();
	}

	return 0;
}