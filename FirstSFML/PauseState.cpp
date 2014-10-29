#include "PauseState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, backgroundSprite()
	, pauseText()
	, instructionText()
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();

	pauseText.setFont(font);
	pauseText.setString("PAUSED");
	pauseText.setCharacterSize(70);
	centerOrigin(pauseText);
	pauseText.setPosition(0.5f * viewSize.x, 0.33f * viewSize.y);

	instructionText.setFont(font);
	instructionText.setString("[BACKSPACE] Main Menu");
	instructionText.setCharacterSize(18);
	instructionText.setPosition(context.window->getView().getSize().x - instructionText.getLocalBounds().width,
		context.window->getView().getSize().y - (2*instructionText.getLocalBounds().height));
}

void PauseState::draw() {
	sf::RenderWindow& win = *getContext().window;
	win.setView(win.getDefaultView());

	sf::RectangleShape bgShape; //background shape
	bgShape.setFillColor(sf::Color(0, 0, 0, 150)); //fourth param (150) is opacity
	bgShape.setSize(sf::Vector2f(win.getSize()));

	win.draw(bgShape);
	win.draw(pauseText);
	win.draw(instructionText);
}

//THIS FALSE RETURN IS IMPORTANT!
//tells the main game loop to STOP updating
//anything under this stack.
bool PauseState::update(sf::Time) {
	return false; 
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape)
	{
		// Escape pressed, remove itself to return to the game
		requestStackPop();
	}

	if (event.key.code == sf::Keyboard::BackSpace)
	{
		// Escape pressed, remove itself to return to the game
		requestStackClear();
		requestStackPush(States::Menu);
	}

	return false;
}