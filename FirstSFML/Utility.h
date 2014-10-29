#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Sprite;
	class Text;
}
namespace GUI {
	class Label;
	class Button;
}
// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

//Statically casts ANY sf::Vector2<T> to an sf::Vector2f
template <typename T>
sf::Vector2f toFloatVector(sf::Vector2<T> v);

//Statically casts ANY sf::Vector2<T> to an sf::Vector2i
template <typename T>
sf::Vector2i toIntVector(sf::Vector2<T> v);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(std::shared_ptr<GUI::Button>& b);
void centerOrigin(std::shared_ptr<GUI::Label>& l);

std::string convertToStr(sf::Keyboard::Key key);
sf::Keyboard::Key convertToKey(const std::string& str);

//Degree/Radian conversions
float toDegree(float radians);
float toRadian(float degrees);

//Vector operations
float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

#include "Utility.inl"
#endif // BOOK_UTILITY_HPP