#include "TitleState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, text()
	, showText(true)
	, textEffectDuration(sf::Time::Zero)
{
	backgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	text.setFont(context.fonts->get(Fonts::Main));
	text.setString("Press any key to continue.");
	centerOrigin(text);
	sf::Vector2f viewSz = context.window->getView().getSize();
	text.setPosition(viewSz.x / 2.f, (2*viewSz.y) / 3.f); // 1/2 x, and 2/3 y (ie, halfway across the screen, and two-thirds the way down)
}

void TitleState::draw() {
	sf::RenderWindow& window = *getContext().window;
	window.draw(backgroundSprite);

	if (showText)
		window.draw(text);
}

bool TitleState::update(sf::Time dt) {
	textEffectDuration += dt;
	if (textEffectDuration >= sf::seconds(0.5f)) {
		showText = !showText;
		textEffectDuration = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& e) {
	if (e.type == sf::Event::KeyReleased || e.type == sf::Event::MouseButtonReleased){
		requestStackPop(); //we request a pop
		requestStackPush(States::Menu); //then we push the new state to be displayed
	}

	return true;
}
