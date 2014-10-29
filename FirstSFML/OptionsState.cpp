#include "OptionsState.h"
#include "Button.h"
#include "Label.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include "SliderBubbles.h"

OptionsState::OptionsState(StateStack& stack, Context context)
	: State(stack, context)
	, GUIContainer(*context.window)
	, opaqueLayer()
{
	auto slider = std::make_shared<GUI::SliderBubbles>("Volume", 10, *context.fonts, *context.textures, *context.window);
	slider->setPosition(sf::Vector2f(context.window->getView().getSize().x /2.f, context.window->getView().getSize().y / 2.f));
	GUIContainer.pack(slider);

	opaqueLayer.setSize(context.window->getView().getSize());
	opaqueLayer.setFillColor(sf::Color(0, 0, 0, 250));

	auto title = std::make_shared<GUI::Label>("SETTINGS", *context.fonts, 40);
	title->setPosition(context.window->getView().getSize().x / 2.f - context.textures->get(Textures::ButtonNormal).getSize().x,
						0.05f*context.window->getView().getSize().y / 10.f);
	GUIContainer.pack(title);

	float xPos = context.window->getView().getSize().x / 10.f;
	float yPos = 3.f * context.window->getView().getSize().y / 10.f;

	auto videoSettings = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	videoSettings->setPosition(xPos, yPos);
	float yInc = videoSettings->getSprite().getLocalBounds().height + 5.f; //5.f is padding between the buttons
	yPos += yInc;
	videoSettings->setText("VIDEO");
	videoSettings->setCallback([this](){
									//requestStackPush(States::VideoSettings);
								});
	GUIContainer.pack(videoSettings);

	auto audioSettings = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	audioSettings->setPosition(xPos, yPos);
	yPos += yInc;
	audioSettings->setText("AUDIO");
	audioSettings->setCallback([this]() {
									//requestStackPush(States::AudioSettings);
								}); 
	GUIContainer.pack(audioSettings);

	auto keybindSettings = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	keybindSettings->setPosition(xPos, yPos);
	yPos += yInc;
	keybindSettings->setText("CONTROLS");
	keybindSettings->setCallback([this]() {
									requestStackPush(States::Settings);
								});
	GUIContainer.pack(keybindSettings);

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(xPos, 0.8f*context.window->getView().getSize().y);	
	backButton->setText("Back");

	backButton->setCallback(std::bind(&OptionsState::requestStackPop, this));
	GUIContainer.pack(backButton);
}

void OptionsState::draw() {
	sf::RenderWindow& win = *getContext().window;
	win.draw(opaqueLayer);
	win.draw(GUIContainer);
}

bool OptionsState::update(sf::Time dt) {
	return true;
}

bool OptionsState::handleEvent(const sf::Event& e) {
	GUIContainer.handleEvent(e);
	return false; //(don't handle events on states beneath this one)
}
