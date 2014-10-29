#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.h"

class PauseState : public State {

public:
	PauseState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& e);

private:
	sf::Sprite backgroundSprite;
	sf::Text pauseText;
	sf::Text instructionText;

};

#endif