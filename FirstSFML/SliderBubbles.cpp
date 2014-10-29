#include "SliderBubbles.h"
#include "Utility.h"

#include "Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
namespace GUI
{

	SliderBubbles::SliderBubbles(std::string nm, size_t bubbleCount, const FontHolder& fonts, const TextureHolder& textures, const sf::Window& win, const float pad)
		: window(win)
		, title(nm, fonts, 14)
		, maxBubbles(bubbleCount)
		, currBubbles(bubbleCount)
		, deadBubble(textures.get(Textures::SliderKnobDead))
		, activeBubble(textures.get(Textures::SliderKnobActive))
		, bubbles()
		, isMousePress(false)
		, bubblePadding(pad)
	{
		float width = 0.f;
		float height = (float)deadBubble.getSize().y;
		for (size_t i = 0; i < maxBubbles; i++) {
			sf::Sprite b;
			b.setTexture(deadBubble);
			//Textures are padded on both sides by half the padding amount (ie, (padding/2) + bubble + (padding/2))
			b.move(sf::Vector2f((bubblePadding / 2.f) + i*((float)deadBubble.getSize().x+(bubblePadding)), title.getText().getLocalBounds().height + 15.f));
			bubbles.push_back(b);
			width += deadBubble.getSize().x + bubblePadding;
		} //fix padding detection!

		for (size_t i = 0; i < currBubbles; i++) {
			bubbles[i].setTexture(activeBubble);
		}

		//all components must set their bounds if they want mouse movement to respond to selection/clicks
		componentBounds.height = height;
		componentBounds.width = width;
		componentBounds.top += title.getText().getLocalBounds().height + 15.f;
	}

	//Selecting a SliderBubbles component also activates it, allowing for input to be redirected to it!
	void SliderBubbles::select() {
		Component::select();
		activate();
	}

	void SliderBubbles::setText(const std::string& str) {
		title.setText(str);
	}

	size_t SliderBubbles::getSelectionCount() const {
		return currBubbles;
	}

	const float SliderBubbles::getMultiplier() {
		return (float)currBubbles / (float)maxBubbles;
	}

	bool SliderBubbles::isSelectable() const {
		return true;
	}

	void SliderBubbles::handleEvent(const sf::Event& e) {
		if (e.type == sf::Event::KeyPressed) {
			switch (e.key.code) {
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
					incBubbles();
					break;

				case sf::Keyboard::A:
				case sf::Keyboard::Left:
					decBubbles();
					break;

				//Calls to deactivate() the component should also set the thing of whatever it's setting
				//ie, this class should have a CALLBACK (std::function) data member as well, which is called with the getMultiplier() parameter.
				//case sf::Keyboard::Enter: //WILL NOT REACH HERE - check comment in Container::handleEvent(..)
				case sf::Keyboard::Escape:
				case sf::Keyboard::W:
				case sf::Keyboard::S:
					isMousePress = false;
					deactivate();
					break;
			}
		}

		else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
			//do some legwork to check if its pressed on the circles.
			if (componentBounds.contains(toFloatVector(sf::Mouse::getPosition(window)))) {
				isMousePress = true;
				//should be handleClick and should behave SLIGHTLY differently than drag.
				handleDrag(sf::Mouse::getPosition(window));
			}
		}
		else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
			//release mousepress regardless
			isMousePress = false;
		}
		else if (e.type == sf::Event::MouseMoved) {
			if (isMousePress){ //to make cleaner, use real time input! isKeyPressed(sf::Mouse::Left)
				handleDrag(sf::Mouse::getPosition(window));
			}
			else{ //deactivate component when we're out of bounds
				if (!componentBounds.contains(toFloatVector(sf::Mouse::getPosition(window)))) { 
					deactivate();
				}
			}
		}
		else if (e.type == sf::Event::LostFocus) {
			isMousePress = false; //removes mouse press when window loses focus
		}
	}

	void SliderBubbles::handleClick(sf::Vector2i mousePos) {
		//TO-DO
		int x = mousePos.x;
		size_t ctr = 0;

		x -= static_cast<int>(componentBounds.left); //remove the empty space to the left of the slider bar so we calculate using 0 as a base
		while (x >= static_cast<int>(deadBubble.getSize().x + bubblePadding)) {
			x -= static_cast<int>(deadBubble.getSize().x + bubblePadding);
			ctr++;
		}
		
	}

	void SliderBubbles::handleDrag(sf::Vector2i mousePos) {
		int x = mousePos.x;
		size_t ctr = 0;

		x -= static_cast<int>(componentBounds.left); //remove the empty space to the left of the slider bar so we calculate using 0 as a base
		while (x >= static_cast<int>(deadBubble.getSize().x + bubblePadding)) {
			x -= static_cast<int>(deadBubble.getSize().x + bubblePadding);
			ctr++;
		}
		if (x >= static_cast<int>((deadBubble.getSize().x + bubblePadding) / 2u)) {
			ctr++;
		}
		//remember, ctr is HOW MANY ACTIVE BUBBLES THERE MUST BE! not index of bubble array
		activateUpTo(std::min(ctr, maxBubbles));
	}

	void SliderBubbles::incBubbles() {
		if (currBubbles < maxBubbles) {
			//since curr < max, that means curr <= max - 1; so it will ALWAYS be a valid index
			bubbles[currBubbles].setTexture(activeBubble);
			currBubbles++;
		}
	}

	void SliderBubbles::decBubbles() {
		if (currBubbles > 0) {
			//currBubbles-1 is the index of the last selected bubble! also guaranteed to be valid a index, since curr > 0, thus curr -1 >= 0
			bubbles[currBubbles - 1].setTexture(deadBubble);
			currBubbles--;
		}
	}

	void SliderBubbles::activateUpTo(size_t limit) {
		//Kills all bubbles
		for (size_t i = 0; i < bubbles.size(); i++){
			bubbles[i].setTexture(deadBubble);
		}
		currBubbles = 0;

		//Activates upto specified bubble.
		for (size_t i = 0; i < limit; i++) {
			bubbles[i].setTexture(activeBubble);
			currBubbles++;
			
		}
	}


	/* OVERRIDDEN FUNCTIONS */
	void SliderBubbles::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(title, states);
		for (size_t i = 0; i < bubbles.size(); i++) {
			target.draw(bubbles[i], states);
		}
	}

	//Notice how these skip the Component setPositions entirely and deal with
	//componentBounds uniquely to SliderBubbles (ie, ensuring that the move/position set drops the box BELOW the SliderBubbles title
	//Follow this methodology if you need to position the mouse hitbox in a unique way (ie, only PART of a component)
	//for other classes
	void SliderBubbles::setPosition(float x, float y) {
		Transformable::setPosition(x, y);
		componentBounds.left = x;
		componentBounds.top = y + title.getText().getLocalBounds().height + 15.f;
	}

	void SliderBubbles::setPosition(const sf::Vector2f& pos) {
		Transformable::setPosition(pos);
		componentBounds.left = pos.x;
		componentBounds.top = pos.y + title.getText().getLocalBounds().height + 15.f;
	}

	void SliderBubbles::move(float offX, float offY){
		Transformable::move(offX, offY);
		componentBounds.left += offX;
		componentBounds.top += offY;
	}
	
	void SliderBubbles::move(const sf::Vector2f& offset) {
		Transformable::move(offset);
		componentBounds.left += offset.x;
		componentBounds.top += offset.y;
	}

}