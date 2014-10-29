#include "Label.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace GUI
{

	Label::Label(const std::string& text, const FontHolder& fonts)
		: txt(text, fonts.get(Fonts::Main), 16)
	{

	}

	Label::Label(const std::string& text, const FontHolder& fonts, int size)
		: txt(text, fonts.get(Fonts::Main), size)
	{

	}

	bool Label::isSelectable() const {
		return false; //label's are not selectable, make a separate SelectLabel class that returns true here!
	}

	void Label::handleEvent(const sf::Event& e) {
		//doesn't handle shit!
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(txt, states);
	}

	void Label::setText(const std::string& text) {
		txt.setString(text);
	}

	sf::Text Label::getText() const {
		return txt;
	}

	void Label::setSFMLText(const sf::Text& text) {
		txt = text;
	}

}