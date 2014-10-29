#include "LoadingState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

LoadingState::LoadingState(StateStack& stack, Context context)
	: State(stack, context)
	, loadStr("Loading Resources")
{
	sf::RenderWindow& window = *getContext().window;
	sf::Font& font = context.fonts->get(Fonts::Main);

	sf::Vector2f viewSize = window.getView().getSize();

	loadingText.setFont(font);
	loadingText.setString(loadStr);
	centerOrigin(loadingText);
	loadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

	progressBackground.setFillColor(sf::Color(40, 40, 40, 125));
	progressBackground.setSize(sf::Vector2f(viewSize.x - 20, 10));
	progressBackground.setPosition(10, loadingText.getPosition().y + 40);

	progressBar.setFillColor(sf::Color(0, 125, 0, 50));
	progressBar.setSize(sf::Vector2f(200, 10));
	progressBar.setPosition(10, loadingText.getPosition().y + 40);

	setCompletion(0.f);
	dotCycleTime = sf::seconds(0.3f);
	elapsedDotTime = sf::seconds(0.f);

	parallelTask.execute();
}

void LoadingState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(loadingText);
	window.draw(progressBackground);
	window.draw(progressBar);
}

bool LoadingState::update(sf::Time dt) {
	if (parallelTask.isFinished()) {
		requestStackPop(); //pops the loading state
		requestStackPush(States::Game);
	}
	else {
		setCompletion(parallelTask.getCompletion());
		elapsedDotTime += dt;
		if (elapsedDotTime >= dotCycleTime) {
			elapsedDotTime -= dotCycleTime;
			cycleDots();
		}
	}
	return true;
}

bool LoadingState::handleEvent(const sf::Event& e) {
	return true; //handle no events, but let event go through the statestack (if there are any others, but there won't be)
}

void LoadingState::setCompletion(float percent) {
	if (percent > 1.f)
		percent = 1.f;//clamp that shit son

	progressBar.setSize(sf::Vector2f(progressBackground.getSize().x * percent, progressBar.getSize().y));
}

void LoadingState::cycleDots() {
	int currDots = (int)loadingText.getString().getSize() - loadStr.length();
	if (currDots < DOT_COUNT)
		setDotCount(++currDots); //increments then uses that value
	else if (currDots == DOT_COUNT)
		setDotCount(0); //ie, MIN DOTS
}

void LoadingState::setDotCount(int dots) {
	std::string str = loadStr;
	if (dots > DOT_COUNT)
		dots = DOT_COUNT;

	for (int i = 0; i < dots; i++)
		str += ".";
	
	loadingText.setString(str);
}

