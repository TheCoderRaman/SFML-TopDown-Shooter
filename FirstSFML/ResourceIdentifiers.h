#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H
namespace sf { //forward decl of SFML class sf::Texture
	class Texture;
	class Font;
}

namespace Textures {
	enum ID { 
		Eagle, 
		Raptor,
		Avenger,
		Desert, 
		TitleScreen,
		ButtonNormal,
		ButtonSelected,
		ButtonPressed,
		SliderKnobActive,
		SliderKnobDead,
		Bullet,
		Missile,
	};
}
namespace Fonts
{
	enum ID {
		Main,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder; //forward decl of OUR resource holder class

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
#endif