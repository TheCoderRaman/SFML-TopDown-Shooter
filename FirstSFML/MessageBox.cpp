#include "MessageBox.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
namespace GUI
{
	MessageBox::MessageBox( Container& partOf,
							const std::string& title,
							const std::string& msg,
							const FontHolder& fonts,
							const TextureHolder& textures,
							const sf::Window& win)
		: Container(win) //ANY component that implements its own container MUST do this!!!!
		, parent(&partOf)
		, titleLabel(std::make_shared<GUI::Label>(title, fonts, 35))
		, messageLabel(std::make_shared<GUI::Label>(msg, fonts, 16))
		, confirmButton(std::make_shared<GUI::Button>(fonts, textures))
		, background()
	{
		activate(); //select the pop up right away (ie, input goes straight to it)

		//NOTE: BACKGROUND position is not specified, cause it will be (0,0)
		//however, we DO SPECIFY the position of the Labels and Buttons, RELATIVE TO THE BACKGROUND
		//and NOT the window.
		opaqueLayer.setSize(sf::Vector2f(win.getSize()));
		opaqueLayer.setOrigin(opaqueLayer.getLocalBounds().width / 2.f, opaqueLayer.getLocalBounds().height / 2.f);
		opaqueLayer.setFillColor(sf::Color(0,0,0,200));

		background.setSize(sf::Vector2f(450.f, 200.f));
		background.setOrigin(background.getLocalBounds().width / 2.f, background.getLocalBounds().height / 2.f);
		background.setFillColor(sf::Color(40, 40, 40));

		centerOrigin(titleLabel);
		float xT = 0.f; //we want the title flush and center!
		float yT = -(background.getLocalBounds().height / 2.f);
		yT += 15.f; //some padding!
		titleLabel->move(sf::Vector2f(xT, yT));
		pack(titleLabel);

		centerOrigin(messageLabel);
		messageLabel->move(sf::Vector2f(0.f, 0.f));
		pack(messageLabel);


		confirmButton->setText("OK");
		confirmButton->setCallback([this]() {
			deactivate();
			parent->pop();
		});
		
		float xB = (background.getLocalBounds().width / 2.f);	//pushes button just RIGHT of pop up
		xB -= confirmButton->getSprite().getLocalBounds().width;	//pushes button JUST BEHIND RIGHT edge of pop up
		xB -= 10.f;												//PADDING!

		float yB = (background.getLocalBounds().height / 2.f);	//push button to bottom of pop up
		yB -= confirmButton->getSprite().getLocalBounds().height;//pushes button JUST ABOVE bottom of pop up
		yB -= 10.f;												//PADDING!

		confirmButton->move(sf::Vector2f(xB, yB)); //we have now, REGARDLESS OF SIZE, anchored the confirm button
		pack(confirmButton);					 //to the 
	}

	void MessageBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(opaqueLayer, states);
		target.draw(background, states);
		for (size_t i = 0; i < children.size(); i++)
			target.draw(*children[i], states);
	}

	bool MessageBox::isSelectable() const {
		return true;
	}

	bool MessageBox::takesFocus() const {
		return true;
	}

}