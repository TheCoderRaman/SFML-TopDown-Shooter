#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State {

public:
	MenuState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& e);

	private://remove this shiat
		void makePopUp(Context context);

private:
	sf::Sprite backgroundSprite;
	GUI::Container GUIContainer;

	int uselessCounter;

};


#endif