#ifndef SLIDER_BUBBLES_H
#define SLIDER_BUBBLES_H

#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>
#include <memory>

#include "Label.h"

//forward decl of container for parent
namespace GUI {
	class Container;
}
namespace GUI
{
	class SliderBubbles : public Component {
	public:
		typedef std::shared_ptr<SliderBubbles> PTR;

	public:
		//Constructs a SliderBubble GUI component. Allows for an end-user to 
		//select the desired amount with an intuitive visual interface.
		//Provides a simple method to retrieve the value of the users choice, 
		//as a percentage of chosen bubbles from total bubbles.
		//Parameters:
		//	- std::string  - Title to be displayed.
		//	- size_t - Number of desired bubbles.
		//	- FontHolder& - FontHolder reference as defined in ResourceIdentifiers.h
		//	- TextureHolder& - Same as above, but with Textures 
		//  - sf::Window& - The main window we're rendering to (used for retrieving relative mouse positions)
		//  - (optional) float - Padding between the bubbles (default is 2 pixels)
		SliderBubbles(std::string nm, 
			size_t bubbleCount,
			const FontHolder& fonts,
			const TextureHolder& textures,
			const sf::Window& win,
			const float pad = 2.f); //note: default parameter! :)
		//Sets the text of the title of the slider.
		void setText(const std::string& str);
		//Returns how many bubbles are currently selected
		size_t getSelectionCount() const;

		//Returns Selected divided by Max bubbles as a float (to be used as a multiplier)
		const float getMultiplier();

		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event& e);
		virtual void select(); //overriding Component::select to call activate() as well

		virtual void setPosition(float x, float y);
		virtual void setPosition(const sf::Vector2f& pos);
		virtual void move(float offX, float offY);
		virtual void move(const sf::Vector2f& offset);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void incBubbles();
		void decBubbles();
		void handleClick(sf::Vector2i mousePos);
		void handleDrag(sf::Vector2i mousePos);
		//Activate's bubbles up to specified number
		void activateUpTo(size_t limit);
		
	private:
		const sf::Window& window;
		const sf::Texture& deadBubble;
		const sf::Texture& activeBubble;

		GUI::Label title;

		std::vector<sf::Sprite> bubbles;
		const size_t maxBubbles;
		size_t currBubbles;
		const float bubblePadding;

		bool isMousePress;
	};
}


#endif