#include "GameState.h"

//This class calls the WORLD draw and update functions! important for the game!!!!!! VERY IMPORTANT STATE!

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, world(*context.window, *context.fonts) //initiates the world object!
	, player(*context.player)
{

}

void GameState::draw() {
	world.draw();
}

bool GameState::update(sf::Time dt) {
	world.update(dt);
	
	CommandQueue& commands = world.getCommandQueue();
	player.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event) {
	CommandQueue& commands = world.getCommandQueue();

	player.handleEvent(event, commands);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);//PAUSE THE GAME! doesn't pop the gamestate!

	return true;
}