#ifndef DISAPPEAR_TEXT_H
#define DISAPPEAR_TEXT_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

class DisappearText : public sf::Text {
public:
	DisappearText(sf::Time duration);
	bool isShowing();
	void reset();
	void addTime(sf::Time dt);

public:
	const sf::Time duration;
private:
	sf::Time elapsed;
	sf::Time timeSinceLastReduc;
	int op_inc;
	int curr_op;

};

#endif