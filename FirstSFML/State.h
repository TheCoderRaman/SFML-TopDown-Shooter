#ifndef STATE_H
#define STATE_H

/*
 * The interface of the individual State.
 * Can request stack operations from StateStack object that each State POINTS to.
 */
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

#include "ResourceIdentifiers.h"
#include "StateIdentifiers.h"

namespace sf {
	class RenderWindow;
}
class Player;
class StateStack;

class State {
	public:
		typedef std::unique_ptr<State> PTR;

		//Since every screen will need to display text or sprites, draw to screen, etc
		//and we want to avoid unnecessary memory wasting by loading the same texture/font to memory in
		//multiple places, we introduce Context struct in each State class
		//Context works as a holder of shared objects between all states of our game.
		struct Context {
			//TO-DO
			Context(sf::RenderWindow& window,
				TextureHolder& textures,
				FontHolder& fonts,
				Player& player);
			sf::RenderWindow* window;
			TextureHolder* textures;
			FontHolder* fonts;
			Player* player;
		};

	public:
		State(StateStack& stack, Context context);
		virtual ~State();

		virtual void draw() = 0; //pure virtual function, derivatives MUST implement this
		virtual bool update(sf::Time dt) = 0;
		virtual bool handleEvent(const sf::Event& e) = 0;

	protected: //derived classes can access this stuff
		//NOTE: These three functions REQUEST an action, and that action is delayed
		//until it is safe to push or pop or clear the StackState pointer.
		//this is because we dont want to alter the stack in the middle of an update iteration
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStackClear(); 

		Context getContext() const;

	private:
		StateStack* Stack;
		Context StateContext;
};



#endif 