#include "Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
namespace GUI
{

	Container::Container(const sf::Window& win)
		: window(win)
		, children()
		, selectedChild(-1)
	{

	}

	void Container::pack(Component::PTR component) {
		children.push_back(component);
		if ((!hasSelection() && component->isSelectable()) || component->takesFocus())//if (!hasSelection() && component->isSelectable())
			select(children.size() - 1);
	}

	//Should only be used for removing popups.
	void Container::pop() {
		selectPrevious();
		children.pop_back();
	}

	void Container::handleEvent(const sf::Event& e) {
		if (hasSelection() && children[selectedChild]->isActive()) {
			children[selectedChild]->handleEvent(e); //child should handle event if possible
		}
		else if (e.type == sf::Event::KeyReleased) {
			if (e.key.code == sf::Keyboard::W || e.key.code == sf::Keyboard::Up) {
				selectPrevious();
			}
			else if (e.key.code == sf::Keyboard::S || e.key.code == sf::Keyboard::Down) {
				selectNext();
			}
			else if (e.key.code == sf::Keyboard::Return || e.key.code == sf::Keyboard::Space) {
				if (hasSelection())
					children[selectedChild]->activate();

				//with this, components will NOT recieve RETURN or SPACE when they're handling events
				//if they're part of a container, and events are handled through the container
				// (ie, GUIContainer.handleEvents(e);
			}
		}
		else if (e.type == sf::Event::MouseMoved) {
			//for each mouse movement, we will loop through all the children components on screen.
			//if the mouse falls into one of the bounds of the components, we will select it (if selectable)
			for (size_t i = 0; i < children.size(); i++) {
				if (children[i]->isSelectable()) {
					if (children[i]->componentBounds.contains(sf::Vector2f(sf::Mouse::getPosition(window)))){
						select(i);
					}
				}
			}
		}
		else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
			//Clicking will check if there is a valid selection, and if so, it will activate the component selected
			if (hasSelection())
				children[selectedChild]->activate();
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();

		for (size_t i = 0; i < children.size(); i++)
			target.draw(*children[i], states);
	}

	bool Container::hasSelection() const {
		return selectedChild >= 0; //default is -1
	}

	bool Container::isSelectable() const {
		return false;
	}

	void Container::select(size_t index) {
		if (children[index]->isSelectable()) {
			if (hasSelection()) //before we select a new container, we deselect any selected ones
				children[selectedChild]->deselect();

			children[index]->select(); //selects the index
			selectedChild = index; //bookkeeping on the index choice!
		}
	}

	void Container::selectNext() {
		if (!hasSelection())
			return; //there's no selection, so select next means NOTHING!
		int next = selectedChild;

		do { //search next component that is selectable, wrap around if necessary
			next = (next + 1) % children.size();
		} while (!children[next]->isSelectable());

		//select the component
		select(next);
	}

	void Container::selectPrevious() {
		if (!hasSelection())
			return;

		int prev = selectedChild;

		do {
			prev = (prev + children.size() - 1) % children.size();
		} while (!children[prev]->isSelectable());
		select(prev);
	}


	size_t Container::childCount() const {
		return children.size();
	}
}
