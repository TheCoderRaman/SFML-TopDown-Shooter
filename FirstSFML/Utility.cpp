#include "Utility.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.h"
#include "Label.h"

void centerOrigin(sf::Sprite& sprite) {
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(sf::Text& text) {
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(std::shared_ptr<GUI::Button>& b) {
	sf::FloatRect bounds = b->getSprite().getLocalBounds();
	b->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(std::shared_ptr<GUI::Label>& l) {
	sf::FloatRect bounds = l->getText().getLocalBounds();
	l->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

template <typename T>
class StringBimap
{
	static_assert(!std::is_same<T, std::string>::value, "Type different from std::string expected");

	public:
		explicit StringBimap(unsigned int valueCount, unsigned int specialValueCount = 0)
			: mStringVector(valueCount + specialValueCount)
			, mTMap()
			, mSpecialValueCount(specialValueCount)
		{
		}

		void insert(T t, const std::string& string)
		{
			mStringVector[t + mSpecialValueCount] = string;
			mTMap[string] = t;
		}

		const std::string& at(T t) const
		{
			std::size_t index = static_cast<std::size_t>(t + mSpecialValueCount);
			if (index >= mStringVector.size() + mSpecialValueCount){
				//throw error	
			}
			else
				return mStringVector[index];
		}

		T at(const std::string& string) const
		{
			auto itr = mTMap.find(string);
			if (itr == mTMap.end()) {

				//throw error	
			}
			else
				return itr->second;
		}

	private:
		std::vector<std::string>	mStringVector;
		std::map<std::string, T>	mTMap;
		unsigned int				mSpecialValueCount;
};

StringBimap<sf::Keyboard::Key> initMap()
{
	StringBimap<sf::Keyboard::Key> bimap(sf::Keyboard::KeyCount, 1);

	#define THOR_INSERT_MAPPING(identifier) bimap.insert(sf::Keyboard::identifier, #identifier)

	THOR_INSERT_MAPPING(Unknown);
	THOR_INSERT_MAPPING(A);
	THOR_INSERT_MAPPING(B);
	THOR_INSERT_MAPPING(C);
	THOR_INSERT_MAPPING(D);
	THOR_INSERT_MAPPING(E);
	THOR_INSERT_MAPPING(F);
	THOR_INSERT_MAPPING(G);
	THOR_INSERT_MAPPING(H);
	THOR_INSERT_MAPPING(I);
	THOR_INSERT_MAPPING(J);
	THOR_INSERT_MAPPING(K);
	THOR_INSERT_MAPPING(L);
	THOR_INSERT_MAPPING(M);
	THOR_INSERT_MAPPING(N);
	THOR_INSERT_MAPPING(O);
	THOR_INSERT_MAPPING(P);
	THOR_INSERT_MAPPING(Q);
	THOR_INSERT_MAPPING(R);
	THOR_INSERT_MAPPING(S);
	THOR_INSERT_MAPPING(T);
	THOR_INSERT_MAPPING(U);
	THOR_INSERT_MAPPING(V);
	THOR_INSERT_MAPPING(W);
	THOR_INSERT_MAPPING(X);
	THOR_INSERT_MAPPING(Y);
	THOR_INSERT_MAPPING(Z);
	THOR_INSERT_MAPPING(Num0);
	THOR_INSERT_MAPPING(Num1);
	THOR_INSERT_MAPPING(Num2);
	THOR_INSERT_MAPPING(Num3);
	THOR_INSERT_MAPPING(Num4);
	THOR_INSERT_MAPPING(Num5);
	THOR_INSERT_MAPPING(Num6);
	THOR_INSERT_MAPPING(Num7);
	THOR_INSERT_MAPPING(Num8);
	THOR_INSERT_MAPPING(Num9);
	THOR_INSERT_MAPPING(Escape);
	THOR_INSERT_MAPPING(LControl);
	THOR_INSERT_MAPPING(LShift);
	THOR_INSERT_MAPPING(LAlt);
	THOR_INSERT_MAPPING(LSystem);
	THOR_INSERT_MAPPING(RControl);
	THOR_INSERT_MAPPING(RShift);
	THOR_INSERT_MAPPING(RAlt);
	THOR_INSERT_MAPPING(RSystem);
	THOR_INSERT_MAPPING(Menu);
	THOR_INSERT_MAPPING(LBracket);
	THOR_INSERT_MAPPING(RBracket);
	THOR_INSERT_MAPPING(SemiColon);
	THOR_INSERT_MAPPING(Comma);
	THOR_INSERT_MAPPING(Period);
	THOR_INSERT_MAPPING(Quote);
	THOR_INSERT_MAPPING(Slash);
	THOR_INSERT_MAPPING(BackSlash);
	THOR_INSERT_MAPPING(Tilde);
	THOR_INSERT_MAPPING(Equal);
	THOR_INSERT_MAPPING(Dash);
	THOR_INSERT_MAPPING(Space);
	THOR_INSERT_MAPPING(Return);
	THOR_INSERT_MAPPING(BackSpace);
	THOR_INSERT_MAPPING(Tab);
	THOR_INSERT_MAPPING(PageUp);
	THOR_INSERT_MAPPING(PageDown);
	THOR_INSERT_MAPPING(End);
	THOR_INSERT_MAPPING(Home);
	THOR_INSERT_MAPPING(Insert);
	THOR_INSERT_MAPPING(Delete);
	THOR_INSERT_MAPPING(Add);
	THOR_INSERT_MAPPING(Subtract);
	THOR_INSERT_MAPPING(Multiply);
	THOR_INSERT_MAPPING(Divide);
	THOR_INSERT_MAPPING(Left);
	THOR_INSERT_MAPPING(Right);
	THOR_INSERT_MAPPING(Up);
	THOR_INSERT_MAPPING(Down);
	THOR_INSERT_MAPPING(Numpad0);
	THOR_INSERT_MAPPING(Numpad1);
	THOR_INSERT_MAPPING(Numpad2);
	THOR_INSERT_MAPPING(Numpad3);
	THOR_INSERT_MAPPING(Numpad4);
	THOR_INSERT_MAPPING(Numpad5);
	THOR_INSERT_MAPPING(Numpad6);
	THOR_INSERT_MAPPING(Numpad7);
	THOR_INSERT_MAPPING(Numpad8);
	THOR_INSERT_MAPPING(Numpad9);
	THOR_INSERT_MAPPING(F1);
	THOR_INSERT_MAPPING(F2);
	THOR_INSERT_MAPPING(F3);
	THOR_INSERT_MAPPING(F4);
	THOR_INSERT_MAPPING(F5);
	THOR_INSERT_MAPPING(F6);
	THOR_INSERT_MAPPING(F7);
	THOR_INSERT_MAPPING(F8);
	THOR_INSERT_MAPPING(F9);
	THOR_INSERT_MAPPING(F10);
	THOR_INSERT_MAPPING(F11);
	THOR_INSERT_MAPPING(F12);
	THOR_INSERT_MAPPING(F13);
	THOR_INSERT_MAPPING(F14);
	THOR_INSERT_MAPPING(F15);
	THOR_INSERT_MAPPING(Pause);

	static_assert(sf::Keyboard::KeyCount == 101, "Number of SFML keys has changed");
	#undef THOR_INSERT_MAPPING

	return bimap;
}

StringBimap<sf::Keyboard::Key> mapInstance() {
	static StringBimap<sf::Keyboard::Key> instance = initMap();
	return instance;
}



std::string convertToStr(sf::Keyboard::Key k) {
	return mapInstance().at(k);
}

sf::Keyboard::Key convertToKey(const std::string& str) {
	return mapInstance().at(str);
}

float toDegree(float radians)
{
	return 180.f / 3.141592653589793238462643383f * radians;
}

float toRadian(float degrees)
{
	return 3.141592653589793238462643383f / 180.f * degrees;
}

float length(sf::Vector2f vector) {
	return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2)); //pow(x, y) == x^y
}

sf::Vector2f unitVector(sf::Vector2f vector) {
	assert(vector != sf::Vector2f(0.f, 0.f));
	return vector / length(vector); // u / |u| (ie, directional vector, bud, jeez, learn yo' math)
}