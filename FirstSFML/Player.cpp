#include "Player.h"
#include "CommandQueue.h"
#include "Aircraft.h"
#include "Utility.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "rapidxml_print.hpp"

struct AircraftMover {
	AircraftMover(float vx, float vy)
		: velocity(vx, vy)
	{

	}

	//Called with constructor, so every time 
	void operator() (Aircraft& aircraft, sf::Time) const {
		aircraft.setVelocity(aircraft.getVelocity() + velocity);
	}

	sf::Vector2f velocity;
};

Player::Player() 
	: speedMult(1.f)
	, cfg()
{
	rapidxml::xml_node<>* root = cfg.doc.first_node();
	rapidxml::xml_node<>* node = root->first_node();

	KeyBindings[convertToKey(node->value())] = MoveUp;
	node = node->next_sibling();

	KeyBindings[convertToKey(node->value())] = MoveDown;
	node = node->next_sibling();

	KeyBindings[convertToKey(node->value())] = MoveLeft;
	node = node->next_sibling();

	KeyBindings[convertToKey(node->value())] = MoveRight;
	node = node->next_sibling();

	KeyBindings[convertToKey(node->value())] = Fire;
	node = node->next_sibling();

	KeyBindings[convertToKey(node->value())] = LaunchMissle;
	//don't forget to next_sibling() when we add more!

	initActions();

	//Set's the commands above as the commands for the player
	for (auto actBind = ActionBindings.begin(); actBind != ActionBindings.end(); actBind++) {
		actBind->second.category = Category::PlayerAircraft;
	}
}

void Player::initActions() {
	ActionBindings[MoveUp].action =
		[=](SceneNode& node, sf::Time dt) {
		node.move(0.f, -(speedMult* player_const::PLAYER_SPEED) * dt.asSeconds()); //negative player speed on Y-axis, ie, moves up
	};
	ActionBindings[MoveDown].action =
		[=](SceneNode& node, sf::Time dt) {
		node.move(0.f, (speedMult* player_const::PLAYER_SPEED) * dt.asSeconds()); //negative player speed on Y-axis, ie, moves up
	};
	ActionBindings[MoveLeft].action =
		[=](SceneNode& node, sf::Time dt) {
		node.move(-(speedMult* player_const::PLAYER_SPEED) * dt.asSeconds(), 0.f); //negative player speed on Y-axis, ie, moves up
	};
	ActionBindings[MoveRight].action =
		[=](SceneNode& node, sf::Time dt) {
		node.move((speedMult* player_const::PLAYER_SPEED) * dt.asSeconds(), 0.f); //negative player speed on Y-axis, ie, moves up
	};

	ActionBindings[Fire].action = derivedAction<Aircraft>([](Aircraft& a, sf::Time){ a.fire(); });
	ActionBindings[LaunchMissle].action = derivedAction<Aircraft>([](Aircraft& a, sf::Time){ a.launchMissile(); });

	//This is MUCH cleaner! But until we define extra actions in Player::AircraftMover, we can only use
	//this method to MOVE  the Player.
	//ActionBindings[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(player_const::PLAYER_SPEED, 0.f));
	//ActionBindings[MoveRight].action = derivedAction<Aircraft>(AircraftMover(player_const::PLAYER_SPEED, 0.f));
	//ActionBindings[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -player_const::PLAYER_SPEED));
	//ActionBindings[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, player_const::PLAYER_SPEED));
}

void Player::assignKey(Action action, sf::Keyboard::Key key) {
	//first, we must remove ALL keys that map to action
	for (auto itr = KeyBindings.begin(); itr != KeyBindings.end();) {
		if (itr->second == action)
			KeyBindings.erase(itr++); //erases itr and moves to next
		else
			++itr;
	}
	//finally, now that all previous bindings to THIS action have been removed, we can set a new one
	KeyBindings[key] = action; //key is the unique identifier in the map!
	Player::writeKeybindToConfig(action, key);
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const {
	for (auto itr = KeyBindings.begin(); itr != KeyBindings.end(); itr++) {
		if (itr->second == action)
			return itr->first; //return the key if we find the action
	}
	return sf::Keyboard::Unknown; //won't reach here if the key is actually found!
}

//Writes KeyBindings to to config file
void Player::writeKeybindToConfig(Action action, sf::Keyboard::Key key) {
	rapidxml::xml_node<>* root = cfg.doc.first_node();

	std::string kStr = convertToStr(key);
	char* val = cfg.doc.allocate_string(kStr.c_str(), strlen(kStr.c_str()));

	std::string name = getActionName(key);
	char* id = cfg.doc.allocate_string(name.c_str(), strlen(name.c_str()));

	root = root->first_node(id);
	root->value(val);

	std::string data;
	rapidxml::print(std::back_inserter(data), cfg.doc);

	try {
		std::ofstream file;
		file.open(cfg.filepath);
		file << data;
		file.close();
	}
	catch (const std::exception& e) {
		std::cout << "EXCEPTION HATH BEEN CAUGHTETH: " << e.what() << std::endl;
	}
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands) {
	if (event.type == sf::Event::KeyPressed) {
		auto found = KeyBindings.find(event.key.code);
		if (found != KeyBindings.end() && !isRealtimeAction(found->second))
			commands.push(ActionBindings[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands) {
	for (auto pair = KeyBindings.begin(); pair != KeyBindings.end(); pair++) {
		if (sf::Keyboard::isKeyPressed(pair->first) && isRealtimeAction(pair->second))
			commands.push(ActionBindings[pair->second]);
	}
}

bool Player::isRealtimeAction(Action action) {
	switch (action) {
		case Action::MoveUp:
		case Action::MoveDown:
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::Fire:
			return true;
		default:
			return false;
	}
}

std::pair<bool, std::string> Player::checkBindOverwrite(sf::Keyboard::Key newKey) {
	try {
		KeyBindings.at(newKey);
		return std::make_pair(true, getActionName(newKey));
	}
	catch (std::exception e) {
		return std::make_pair(false, "");
	}
}

void Player::setSpeedMult(float mult) {
	speedMult = mult;
}

//Returns the name of the action that is bound to the specified key.
//If no action is found corresponding to the specified key, we simply return an empty string.
std::string Player::getActionName(sf::Keyboard::Key k) {
	if (getAssignedKey(Action::MoveUp) == k) {
		return "MoveUp";
	}
	else if (getAssignedKey(Action::MoveDown) == k) {
		return "MoveDown";
	}
	else if (getAssignedKey(Action::MoveLeft) == k) {
		return "MoveLeft";
	}
	else if (getAssignedKey(Action::MoveRight) == k) {
		return "MoveRight";
	}
	else if (getAssignedKey(Action::Fire) == k) {
		return "Fire";
	}
	else if (getAssignedKey(Action::LaunchMissle) == k) {
		return "LaunchMissile";
	}
	//EXTRA ACTIONS SHOULD GO HERE
	else
		return "";
}
