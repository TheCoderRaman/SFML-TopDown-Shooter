
#include "StateStack.h"
#include <cassert>

StateStack::StateStack(State::Context context)
: Stack()
, PendingList()
, Context(context)
, Factories()
{

}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}

void StateStack::draw() {
	for (auto itr = Stack.begin(); itr != Stack.end(); ++itr) {
		(*itr)->draw();
	}
}

void StateStack::update(sf::Time dt) {
	for (auto itr = Stack.rbegin(); itr != Stack.rend(); ++itr) {
		if (!(*itr)->update(dt))
			break;
	}
	
	applyPendingChanges();
}

//try implementing that FOREACH shit
void StateStack::handleEvent(const sf::Event& e) {
	//starts at reverse beginning (ie end), all the way to reverse end (ie beginning)
	//so, it goes backwards from end->beginning
	for (auto itr = Stack.rbegin(); itr != Stack.rend(); ++itr) {
		if (!(*itr)->handleEvent(e))
			break;
	}
	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	PendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	PendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	PendingList.push_back(PendingChange(Clear));
}

//For archeological purposes: how to communicate between states:
//void StateStack::pushStateAndContext(States::ID stateID, State::Context context) {
//	PendingList.push_back(PendingChange(PushWithContext, stateID, context));
//}

bool StateStack::isEmpty() const
{
	return Stack.empty();
}

//Returns a unique_ptr to newly created State obj.
//It does so by looking up the ID in the map (Factories) and invoking the stored std::function factor, which returns the ptr to the State base class
State::PTR StateStack::createState(States::ID stateID) {
	auto found = Factories.find(stateID);
	assert(found != Factories.end());
	return found->second(); //cause second is a function in Factories!
}


void StateStack::applyPendingChanges() {
	//for each pending change in the pending list
	for (auto itr = PendingList.begin(); itr != PendingList.end(); ++itr) {
		switch (itr->action) {
			case Action::Push:
				Stack.push_back(createState(itr->stateID));
				break;

			case Action::Pop:
				Stack.pop_back();
				break;

			case Action::Clear:
				Stack.clear();
				break;

			/* For archeological purposes: how to communicate between states
			case Action::PushWithContext:
				Stack.push_back(std::unique_ptr<State>(new KeybindState(*this, itr->keybindContext)));
				break;*/

		}
	}
	PendingList.clear(); //clearing the PendingList after we've done all the pending changes!
}
