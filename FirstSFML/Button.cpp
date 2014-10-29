#include "Button.h"
#include "Utility.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	Button::Button(const FontHolder& fonts, const TextureHolder& textures)
		: callback()
		, tNormal(textures.get(Textures::ButtonNormal))
		, tSelected(textures.get(Textures::ButtonSelected))
		, tPressed(textures.get(Textures::ButtonPressed))
		, sprite()
		, text("", fonts.get(Fonts::Main), 16)
		, isToggle(false)
	{
		sprite.setTexture(tNormal);

		componentBounds = sprite.getLocalBounds();
		text.setPosition(componentBounds.width / 2.f, componentBounds.height / 2.f);
	}

	void Button::setCallback(CALLBACK cb) {
		callback = std::move(cb);
	}

	void Button::setText(const std::string& str) {
		text.setString(str);
		centerOrigin(text);
	}

	void Button::setToggle(bool flag) {
		isToggle = flag;
	}

	bool Button::isSelectable() const {
		return true;
	}

	void Button::select() {
		Component::select();
		sprite.setTexture(tSelected);
	}

	void Button::deselect() {
		Component::deselect();
		sprite.setTexture(tNormal);
	}

	void Button::activate() {
		Component::activate();
		//if we are toggle, then we should show that its been pressed
		if (isToggle) //if its a togglable button
			sprite.setTexture(tPressed); //toggle-woggle that shit son
		
		if (callback) //if there's a function to call
			callback(); //call that shit son!

		//if we are not a toggle, then deactivate the button since we are just momentarily activated
		//and then deactivated after the callback has been called back.
		if (!isToggle)
			deactivate();
	}

	void Button::deactivate() {
		Component::deactivate();

		if (isToggle) {
			if (isSelected())
				sprite.setTexture(tSelected);
			else
				sprite.setTexture(tNormal);
		}
	}

	void Button::handleEvent(const sf::Event& e){
		//don't handle nada
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(sprite, states);
		target.draw(text, states);
	}

	const sf::Sprite& Button::getSprite() {
		return sprite;
	}

}