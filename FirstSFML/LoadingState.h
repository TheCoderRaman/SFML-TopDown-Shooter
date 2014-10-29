#ifndef LOADING_STATE_H
#define LOADING_STATE_H

#include "State.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ParallelTask.h"

class LoadingState : public State {

public:
	LoadingState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& e);

	void setCompletion(float percent);

private:
	sf::Text loadingText;
	std::string loadStr;
	const int DOT_COUNT = 3; //this is the only thing that controls how many dots to use! :)

	sf::RectangleShape progressBackground;
	sf::RectangleShape progressBar;
	ParallelTask parallelTask;

	sf::Time dotCycleTime;
	sf::Time elapsedDotTime;

	private:
		void setDotCount(int dots);
		void cycleDots();
};


#endif