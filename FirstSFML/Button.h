#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace GUI
{
	class Button : public Component {
	public:
		typedef std::shared_ptr<Button> PTR;
		//Callbacks are cool! We want buttons to execute shit when pressed. Instead
		//of querying the button if it has been pressed, we use callbacks using std::function.
		//When the button is activated by the GUI container, we simply execute the callback!
		typedef std::function<void()>	CALLBACK;

	public:
		Button(const FontHolder& fonts, const TextureHolder& textures);
		void setCallback(CALLBACK cb);
		void setText(const std::string& str);
		void setToggle(bool flag);

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();

		virtual void activate();
		virtual void deactivate();

		virtual void handleEvent(const sf::Event& e);

		const sf::Sprite& getSprite(); //returns a reference to a constant sprite.

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		CALLBACK callback;
		const sf::Texture& tNormal;
		const sf::Texture& tSelected;
		const sf::Texture& tPressed;
		sf::Sprite sprite;
		sf::Text text;
		bool isToggle;

	};
}

#endif