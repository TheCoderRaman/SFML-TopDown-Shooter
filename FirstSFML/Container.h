#ifndef CONTAINER_H
#define CONTAINER_H


#include "Component.h"


#include <vector>
#include <memory>
namespace sf {
	class Window;
}
namespace GUI 
{


	class Container : public Component {
		public:
			typedef std::shared_ptr<Container> PTR;
		public:
			Container(const sf::Window& win);
			void pack(Component::PTR component);
			void pop();
			virtual bool isSelectable() const;
			virtual void handleEvent(const sf::Event& e);
			size_t childCount() const;

		private:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			bool hasSelection() const;
			void select(std::size_t index);
			void selectNext();
			void selectPrevious();

		private:
			const sf::Window& window;
			int selectedChild;
		
		protected:
			std::vector<Component::PTR> children; //a vector of shared_ptr's to components
	};



}

#endif