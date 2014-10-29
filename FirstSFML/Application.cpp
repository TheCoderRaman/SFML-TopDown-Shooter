#include "Application.h"
#include "State.h"
#include "StateIdentifiers.h"

#include "TitleState.h"
#include "MenuState.h"
#include "OptionsState.h"
#include "LoadingState.h"
#include "GameState.h"
#include "PauseState.h"
#include "SettingsState.h"

#include "Utility.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
: mWindow(sf::VideoMode(640, 480), "PEWPEWTHINGS!", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);
	//mWindow.setFramerateLimit(200);

	mFonts.load(Fonts::Main, "Media/Font/Sansation.ttf");
	mTextures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
	//CONSIDER moving to World::loadTextures()
	mTextures.load(Textures::ButtonNormal, "Media/Textures/ButtonNormal.png");
	mTextures.load(Textures::ButtonSelected, "Media/Textures/ButtonSelected.png");
	mTextures.load(Textures::ButtonPressed, "Media/Textures/ButtonPressed.png");
	mTextures.load(Textures::SliderKnobActive, "Media/Textures/SliderKnobActive.png");
	mTextures.load(Textures::SliderKnobDead, "Media/Textures/SliderKnobDead.png");
	
	//ENDCONSIDER
	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	//STARTS THE TITLE SCREEN!
	mStateStack.pushState(States::Title);
}

void Application::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen()) {
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			//closing the game when there are no more states left.
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput() {
	sf::Event e;
	while (mWindow.pollEvent(e)) {
		mStateStack.handleEvent(e);

		if (e.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt) {
	mStateStack.update(dt);
}

void Application::render() {
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt) {
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

//important! add all states to this!
void Application::registerStates() {
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<OptionsState>(States::Options);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<LoadingState>(States::Loading);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
}

