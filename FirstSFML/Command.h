#ifndef COMMAND_H
#define COMMAND_H


/*
 * The Command interface. Defines the Command structure,
 * and utilizes C++11 lambda functions to dispatch commands.
 */
#include "Category.h"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode; 

struct Command {
	Command();
	std::function<void(SceneNode&, sf::Time)>	action;
	unsigned int category;
};

//black magic with lamda functions
template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}
#endif // COMMAND_H