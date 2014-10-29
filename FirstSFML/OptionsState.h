#ifndef OPTIONS_STATE_H
#define OPTIONS_STATE_H

#include "State.h"
#include "Container.h"
#include <SFML/Graphics/RectangleShape.hpp>
class OptionsState : public State {
	public:
		OptionsState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& e);

	private:
		GUI::Container GUIContainer;
		sf::RectangleShape opaqueLayer;
};

#endif