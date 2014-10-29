#include "State.h"
#include "StateStack.h"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, player(&player)
{

}

State::State(StateStack& stack, Context context)
	: Stack(&stack)
	, StateContext(context)
{

}

State::~State() {

}

void State::requestStackPush(States::ID stateID) {
	Stack->pushState(stateID);
}
//ooverload requestStackPush to also accept a size_t (enum Action category)

//For Archaeological purposes: How to communicate inter-state.
//void State::requestStackPush(States::ID stateID, Context context) {
//	Stack->pushStateAndContext(stateID, context);
//}
void State::requestStackPop() {
	Stack->popState();
}

void State::requestStackClear() {
	Stack->clearStates();
}

State::Context State::getContext() const {
	return StateContext;
}
