#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H

#include "State.h"
#include "Player.h"
#include "Container.h"
#include "Button.h"
#include "Label.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <array>


class SettingsState : public State
{
public:
	SettingsState(StateStack& stack, Context context);

	virtual void					draw();
	virtual bool					update(sf::Time dt);
	virtual bool					handleEvent(const sf::Event& event);


private:
	void							updateLabels();
	void							addButtonLabel(Player::Action action, float y, const std::string& text, Context context);
	void							initLines(Context context);
private:
	std::vector<sf::RectangleShape>						lines;
	GUI::Container										mGUIContainer;
	GUI::Label::PTR											title;
	std::array<GUI::Button::PTR, Player::ActionCount>	mBindingButtons;
	std::array<GUI::Label::PTR, Player::ActionCount> 	mBindingLabels;

	float bLineLength = 30.f;
	float bLineWidth = 2.f;
	float padding = 5.f;
};

#endif