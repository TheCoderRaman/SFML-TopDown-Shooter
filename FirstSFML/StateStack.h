#ifndef STATE_STACK_H
#define STATE_STACK_H

/*
 * The stack of states which manages all the different screens and transitions between certain screens in game.
 * ie, Main Menu, Game Screen, Pause Screen, Introduction States (vid/logos/opening artwork).
 * Every polled event is fed to the state stack. Then internally, the stack will deliver that event to the active states
 */

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <map>
#include <functional>
#include <utility>

#include "State.h"
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

class StateStack : private sf::NonCopyable {
	public:
		enum Action{
			Push,
			Pop,
			Clear,
			//PushWithContext,
		};

	public:
		explicit StateStack(State::Context context);


		//inserts Function Factory mappings (int Factories map variable).
		//Parameter T is the derived state class we want to register
		template <typename T>
		void registerState(States::ID);


		void update(sf::Time dt);
		void draw();
		void handleEvent(const sf::Event& e);

		void pushState(States::ID stateID);
		void popState();
		void clearStates();
		//void pushStateAndContext(States::ID stateID, State::Context context);

		bool isEmpty() const;

	private:
		//Takes an ID of a state, and returns a unique_ptr to a newly created obj of the corresponding State class.
		State::PTR createState(States::ID stateID);
		void applyPendingChanges();

	private:
		struct PendingChange {
			//TO-DO
			explicit PendingChange(Action action, States::ID stateID = States::None);
			//PendingChange(Action action, States::ID stateID, State::Context con);
			Action action;
			States::ID stateID;
			//State::Context keybindContext; //we have a separate context for this
			//because we're forwarding a context from the previous State (Options) to the next State (Keybinding) - this context
			//tells us which action we're changing
		};

	private:
		std::vector<State::PTR> Stack;
		std::vector<PendingChange> PendingList;
		State::Context	Context;
		//Maps State IDs to factory functions that create a new state on-demand
		std::map<States::ID, std::function<State::PTR()>> Factories;


};

template <typename T>
void StateStack::registerState(States::ID stateID) {
	Factories[stateID] = [this]() { //FACTORIES MAP ADDED HERE
		return State::PTR(new T(*this, Context)); //Means, derived states MUST accept 2 params, a StateStack, and a Context obj
	};
}
#endif