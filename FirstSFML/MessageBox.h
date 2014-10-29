#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include "Container.h"
#include "Button.h"
#include "Label.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace GUI
{

	class MessageBox : public Container {
		public:
			typedef std::shared_ptr<MessageBox> PTR;

		public:
			MessageBox(Container& partOf,
				const std::string& title,
				const std::string& msg,
				const FontHolder& fonts,
				const TextureHolder& textures,
				const sf::Window& win);


			virtual bool isSelectable() const;

		private:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			virtual bool takesFocus() const;

		private:
			sf::RectangleShape opaqueLayer;
			sf::RectangleShape background;

			GUI::Container* parent;
			GUI::Label::PTR titleLabel;
			GUI::Label::PTR messageLabel;
			GUI::Button::PTR confirmButton;
	};
}

#endif

/* 
IMPORTANT INFO ABOUT MESSAGE BOX:

 - Derived from container. it is essentially a container. A container is derived from a COMPONENT

 - COMPONENT DEFINES:
	- takesFocus() - by default, ALL components are false by default.
				   - only message boxes and such should take focus.
	- activate()/deactivate()

 - CONTAINER DEFINES:
	- the select logic (select/selectNext/selectPrevious)
	- pack/pop.

 - MESSAGE BOXES/ETC MUST DEFINE (to behave appropriately with input and focus)
	- virtual bool isSelectable() const { return true; }
	- virtual bool takesFocus() const { return true; }

 - Container vs MessageBox:
	- Container is NOT selectable, MessageBox IS selectable
	- Container DOES NOT take focus, MessageBox DOES take focus

 - LIFETIME ROUTE:
	- MessageBox created
	- MessageBox position set
	- MessageBox packed into GUIContainer.

 - WHen created
	- a MessageBox is instantly activated: in MessageBox::MessageBox();
	- a MessageBox is instantly selected: in Container::pack(Component::PTR) -->automatically select's last added component (if selectable)
	- All its buttons and label placement is placed accordingly and packed, as well as button logic
	- It is drawn in the following order opaqueLayer->background->children

 - When destroyed (ie, OK button ENTERed)
	- MessageBox is deactivate()'d (ie, instantly losing input focus)
	- MessageBox is popped off of the parent's GUI container
		- note: this works without having to specify an erase index
				because in ALL cases, a user will be presented with a popup, and WAIT for his input
				ensuring that it was the last component added to the GUIContainer.
				- double note: i tested multiple popups in succession and it works grovily!

*/