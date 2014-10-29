#ifndef LABEL_H
#define LABEL_H

#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	class Label : public Component {

	public:
		typedef std::shared_ptr<Label> PTR;

	public:
		//Creates a label with the default size of 16pts.
		//Params - `const std::string&` ->The text to display
		//		 - `const FontHolder&` ->The fontholder.
		Label(const std::string& text, const FontHolder& fonts);
		//Creates a label, however, allows a user-defined size.
		//Params - `const std::string&` ->The text to display
		//		 - `const FontHolder&` ->The fontholder.
		//		 - `int` ->The desired size of the label.
		Label(const std::string& text, const FontHolder& fonts, int size);
		
		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event& e);

		sf::Text getText() const;
		void setText(const std::string& text);
		void setSFMLText(const sf::Text& text);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Text txt;


	};
}

#endif