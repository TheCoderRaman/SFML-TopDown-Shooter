#include "MenuState.h"
#include "Button.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include "Player.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "MessageBox.h"

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, GUIContainer(*context.window)
	, uselessCounter(0)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Main);

	backgroundSprite.setTexture(texture);
	//centering positions manually (centerOrigin() fucks with mousepos detection.
	const float xPos = context.window->getView().getSize().x / 2.f - (context.textures->get(Textures::ButtonNormal).getSize().x / 2u);
	float yPos = context.window->getView().getSize().y / 2.f - (context.textures->get(Textures::ButtonNormal).getSize().y / 2u);
	//auto = std::shared_ptr<GUI::Component>
	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	playButton->setPosition(xPos, yPos);
	playButton->setText("Play!");
	playButton->setCallback([this]() {
								requestStackPop();
								requestStackPush(States::Loading);
							});
	//centerOrigin(playButton);
	GUIContainer.pack(playButton);
	float yIncrement = 1.1f * playButton->getSprite().getLocalBounds().height; // 50.f;
	yPos += yIncrement;

	auto settingsButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	settingsButton->setPosition(xPos, yPos);
	settingsButton->setText("Options");
	settingsButton->setCallback([this]() {
									requestStackPush(States::Options);
								});
	//centerOrigin(settingsButton);
	GUIContainer.pack(settingsButton);
	yPos += yIncrement;

	auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	exitButton->setPosition(xPos, yPos);
	exitButton->setText("Exit (like a loser would)");
	exitButton->setCallback([this]() {
								requestStackPop(); //exits game
							});
	//centerOrigin(exitButton);
	GUIContainer.pack(exitButton);
	yPos += yIncrement;

	auto uselessButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	uselessButton->setPosition(xPos, yPos);
	uselessButton->setText("Useless!");
	uselessButton->setCallback([=]() {
								uselessCounter++;
								if (uselessCounter == 10) {
									context.player->setSpeedMult(2.f);

									makePopUp(context);
								}
							});
	//centerOrigin(uselessButton);
	GUIContainer.pack(uselessButton);
	yPos += yIncrement;

	//makePopUp(context);
}

void MenuState::draw() {
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView()); //try commenting this out
	
	window.draw(backgroundSprite);
	window.draw(GUIContainer);
}

bool MenuState::update(sf::Time dt) {
	return true;
}

bool MenuState::handleEvent(const sf::Event& e) {
	GUIContainer.handleEvent(e);
	return false;
}

void MenuState::makePopUp(Context context) {
	std::shared_ptr<GUI::MessageBox> d = std::make_shared<GUI::MessageBox>(GUIContainer,
																				"SECRET",
																				"Cheat activated, you persistant sum'bitch! 2x Speed Boost!",
																				*context.fonts,
																				*context.textures,
																				*context.window);
	d->setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y / 2.f);
	GUIContainer.pack(d);
}

