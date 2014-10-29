#include "DisappearText.h"
#include <iostream>
#include <SFML/Graphics/Color.hpp>
DisappearText::DisappearText(sf::Time duration)
	: duration(duration)
	, elapsed(sf::seconds(0.f))
	, op_inc(2)
	, curr_op(255)
	, timeSinceLastReduc(sf::seconds(0.f))
{
}
//fix this, it disappears instantly like a gay
bool DisappearText::isShowing(){
	return elapsed < duration;
}

void DisappearText::reset() {
	elapsed = sf::seconds(0);
}

void DisappearText::addTime(sf::Time dt) {
	elapsed += dt;
	timeSinceLastReduc += dt;
	if (elapsed <= duration) {
		if (timeSinceLastReduc > (0.5f*duration) / 50.f) {
			timeSinceLastReduc -= (0.5f*duration) / 50.f;
			curr_op -= op_inc;
			setColor(sf::Color(curr_op, curr_op, curr_op, curr_op));
		}
	}
}