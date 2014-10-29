#ifndef UTILITY_INL
#define UTILITY_INL

template <typename T>
std::string toString(const T& value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}

template <typename T>
sf::Vector2f toFloatVector(sf::Vector2<T> v) {
	return sf::Vector2f(static_cast<float>(v.x), static_cast<float>(v.y));
}

template <typename T>
sf::Vector2i toIntVector(sf::Vector2<T> v) {
	return sf::Vector2i(static_cast<int>(v.x), static_cast<int>(v.y));
}

#endif