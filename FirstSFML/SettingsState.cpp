#include "SettingsState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer(*context.window)
	, lines()
	, title(std::make_shared<GUI::Label>("/Keybindings", *context.fonts, 40))
{
	title->setPosition(context.window->getView().getSize().x / 2.f, 0.05f*context.window->getView().getSize().y / 10.f);
	mGUIContainer.pack(title);

	initLines(context);
	// Build key binding buttons and labels
	float y = 3.f * context.window->getView().getSize().y / 10.f;
	float yInc = context.textures->get(Textures::ButtonNormal).getSize().y + padding;
	addButtonLabel(Player::MoveUp, y, "Move Up", context);
	y += yInc;
	addButtonLabel(Player::MoveDown, y, "Move Down", context);
	y += yInc;
	addButtonLabel(Player::MoveLeft, y, "Move Left", context);
	y += yInc;
	addButtonLabel(Player::MoveRight, y, "Move Right", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(context.window->getView().getSize().x / 2.f, 375.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

void SettingsState::initLines(Context context) {
	//line len should be (numOptions-1)* .... (we have 4 options, thus we multiply by 3)
	float lineLen = 3 * (context.textures->get(Textures::ButtonNormal).getSize().y + padding) + bLineWidth;
	sf::RectangleShape firstLine(sf::Vector2f(lineLen, 2.f));
	firstLine.rotate(90.f);
	float yPos = (3.f * context.window->getView().getSize().y / 10.f) + (context.textures->get(Textures::ButtonNormal).getSize().y / 2.f);
	firstLine.setPosition(context.window->getView().getSize().x / 2.f - 30.f, yPos); //line length!
	lines.push_back(firstLine);

	float connectLen = firstLine.getPosition().x - (context.textures->get(Textures::ButtonNormal).getSize().x+context.window->getView().getSize().x / 10.f);
	sf::RectangleShape connector(sf::Vector2f(connectLen, bLineWidth));
	float cX = (context.window->getView().getSize().x / 10.f) + context.textures->get(Textures::ButtonNormal).getSize().x;
	float cY = (3.f * context.window->getView().getSize().y / 10.f) + (2.5f * context.textures->get(Textures::ButtonNormal).getSize().y) + (2 * padding);
	connector.setPosition(cX, cY);
	lines.push_back(connector);

}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mGUIContainer);
	for (size_t i = 0; i < lines.size(); i++){
		window.draw(lines[i]);
	}
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < Player::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);

	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < Player::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		mBindingLabels[i]->setText(convertToStr(key));
	}
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{

	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[action]->setPosition(context.window->getView().getSize().x/2.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[action]->setPosition(300.f, y + 15.f);
	mBindingLabels[action]->setPosition(mBindingButtons[action]->getPosition().x + mBindingButtons[action]->getSprite().getLocalBounds().width + 15.f, y + 15.f);
	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);

	//draw line
	sf::RectangleShape line(sf::Vector2f(bLineLength, bLineWidth));
	line.setPosition(mBindingButtons[action]->getPosition().x - bLineLength, y +(mBindingButtons[action]->getSprite().getLocalBounds().height / 2.f));
	lines.push_back(line);
}