#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "State.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
class TitleState : public State {
public:
	TitleState(StateStack& stack,
		Context context);

	virtual void draw();
	virtual bool update(sf::Time dt); 
	virtual bool handleEvent(const sf::Event& e);

private:
	sf::Sprite	backgroundSprite;
	sf::Text	text;

	bool		showText;
	sf::Time	textEffectDuration;
};

#endif