#ifndef COMPONENT_H
#define COMPONENT_H

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory> //shared_ptr

namespace sf {
	class Event;
}

namespace GUI
{
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
		public:
			typedef std::shared_ptr<Component> PTR;

		public:
								Component();
			virtual				~Component();

			virtual bool		isSelectable() const = 0;
			bool				isSelected() const;
			virtual void		select();
			virtual void		deselect();

			virtual bool		isActive() const;
			virtual void		activate();
			virtual void		deactivate();

			virtual void		handleEvent(const sf::Event& event) = 0;

			virtual bool takesFocus() const;

			//Overriding Transformable:: functions
			virtual void setPosition(float x, float y);
			virtual void setPosition(const sf::Vector2f& pos);
			virtual void move(float offX, float offY);
			virtual void move(const sf::Vector2f& offset);

			//The bounds of the component. Important for mouse-related events which are sensitive to mouse position.
			//All the book keeping on this is done internally when setPosition or move is called.
			//The float values in this FloatRect SHOULD NEVER BE ALTERED OUTSIDE OF .setPosition() or .move()
			//or you risk introducing errors to mouse-detection (ie, mouse over components)
			//This should only ever be altered when introducing a custom mouse hitbox location for the component,
			//and you're overriding Component::setPosition() & Component::move() straight to the Transformable:: ones.
			//Furthermore, it's specifications (left,top,height,width) MUST be specified at creation in ctor (and setPosition/move)
			//will take care of the rest.
			sf::FloatRect componentBounds;

		private:
			bool selected;
			bool active;

	};
}

#endif