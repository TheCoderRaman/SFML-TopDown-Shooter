#include "Component.h"

namespace GUI
{
	Component::Component()
		: selected(false)
		, active(false)
		, componentBounds()
	{

	}

	Component::~Component()
	{
	}

	bool Component::isSelected() const {
		return selected;
	}

	void Component::select() {
		selected = true;
	}

	void Component::deselect() {
		selected = false;
	}

	bool Component::isActive() const {
		return active;
	}

	void Component::activate() {
		active = true;
	}

	void Component::deactivate() {
		active = false;
	}

	bool Component::takesFocus() const {
		return false; //default (ie abstract) containers DO NOT take focus. Only physical containers like message boxes do
	}

	void Component::setPosition(float x, float y) {
		Transformable::setPosition(x, y);
		componentBounds.left = x;
		componentBounds.top = y;
	}

	void Component::setPosition(const sf::Vector2f& pos) {
		Transformable::setPosition(pos);
		componentBounds.left = pos.x;
		componentBounds.top = pos.y;
	}

	void Component::move(float offX, float offY) {
		Transformable::move(offX, offY);
		componentBounds.left += offX;
		componentBounds.top += offY;
	}

	void Component::move(const sf::Vector2f& offset){
		Transformable::move(offset);
		componentBounds.left += offset.x;
		componentBounds.top += offset.y;
	}

}
