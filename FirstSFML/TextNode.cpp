#include "TextNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text) {
	displayText.setFont(fonts.get(Fonts::Main));
	displayText.setCharacterSize(20);
	setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(displayText, states);
}

void TextNode::setString(const std::string& text) {
	displayText.setString(text);
	centerOrigin(displayText);
}