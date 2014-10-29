#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Window/Event.hpp>
#include "Command.h"
#include <map>
#include "ConfigReader.h"

/*
* This is the standard Player class. Note, it is not drawn, or extended from Entity because it is NOT an entity.
* This class will essentially be the PlayerController for the player.
* It handles ALL the input that affects the PLAYER.
*/

namespace player_const { //maybe default keys go here!
	const float PLAYER_SPEED = 300.f;
}

class CommandQueue;
class Player{
	public:
		enum Action {
			MoveUp, 
			MoveDown,
			MoveLeft,
			MoveRight,
			Fire,
			LaunchMissle,
			ActionCount //gives number of Enums that come before it (ie 5) - useful for cycling through actions
		};

		Player();
		void assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key getAssignedKey(Action action) const;
		std::string getActionName(sf::Keyboard::Key k);

		void	handleEvent(const sf::Event& event, CommandQueue& commands);
		void	handleRealtimeInput(CommandQueue& commands);
		
		//std::pair<bool, std::string>	checkBindOverwrite(Action act, sf::Keyboard::Key newKey);
		std::pair<bool, std::string>	checkBindOverwrite(sf::Keyboard::Key newKey);

		void	setSpeedMult(float mult);

private:
	static bool isRealtimeAction(Action action);
	void		initActions();
	void		writeKeybindToConfig(Action action, sf::Keyboard::Key key);

private:
	std::map<sf::Keyboard::Key, Action> KeyBindings;
	std::map<Action, Command>			ActionBindings;
	float speedMult;
	ConfigReader cfg;
};

#endif
/*
 * READ:
 * We've gotten a little messy with the actions thanks to keybinding.
 * Here's what must be done when adding in a new action:
 
 * 1. Add enum identifier to Player::Action enum, just above ActionCount
       *NOTE: Do NOT break the order, but make sure its above ActionCount

 * 2. Add the name of the action and its value to the XML file
	  MUST ENSURE THAT THE XML ORDER IS EQUAL TO THE ORDER IN THE ENUM
	  (Actually, I'm pretty sure it's fine since we're using .find_node("name")
	  instead of iterating with .next_sibling()). But don't fuck around. Just keep it in the same order)
       * NOTE: it's not necessary for the name of the node in the XML file
	   ******* to match, but it's preferred, for when we use rapidxml parser
	   ******* we can just use .find_node("MoveUp") - so it will get
	   ******* veritably confusing if we .find_node("Forward") to set the MoveUp action
	   ******* it's just easier on the eyes!

 * 3. Now that the value is in the XML file, set the Keybinding of the action
      in the Player constructor, where all the other keys are bound
	  during initialization.

 * 4. Init the action in Player::initActions() - (ie, specify its
	  behaviour!)

 * 5. Player::isRealtimeAction(Action action) should have a new 
      "case Action::NewActionName" which returns true with the others.
	    * NOTE: If it's a gameplay element, it should be real time

* 6. Player::getActionName(Keyboard::Key) needs its name
     assigned to the action. This is LITERALLY only used when
	 we check if a user's specified key bind is going to overwrite
	 another keybind, at which point we'll warn the user that he's
	 rebinding the specified action (ie, the result of this string)
	    * Since Player::Action is an enum and doesn't have a string
		  I elected to do it simply like this. A little extra work,
		  cause string enum's are the devil.

*** OptionsState ***
 
 * 7. a) OptionsState::optionNames vector should include the action name (done similarly to how is presented in the code)
      b) OptionsState::optionChoices vector should include the keybind 
		(done similarly to how is presented in the code, ie, look to
		fOpt, bOpt, lOpt, rOpt)

	    * NOTE: It is very important that ORDER is maintained here.
		   ** It literally must be the EXACT order of the enum (minus ActionCount)
		   ** Because we're using the index of the users selection (of which actual key to bind)
		   ** as the index of the Action (enum) of the action we wish to rebind.
		   ** This is SO IMPORTANT, because we write AND READ to the file in order
				** ie, user selects the 2nd key to rebind (ie, 2nd of WSAD = S)
				**     and we pass that value (actually, 1 [0, 1] is the second val)
				**    so now, the size_t INDEX of 1 will be used to reference Player::Action
				**    Player::Action::MoveDown = 1, and we have now identified the key the
				**    user wishes to rebind. THATS WHY THE ORDER IS IMPORTANT!

	c) OptionsState::updateKeyValues() should include our new action

*** KeybindState ***

 * 8. Add logic of new action to the switch statement in KeybindState::finalizeBind(std::string)
 
 * 9. Add new action case to the switch statement in KeybindState::initText(size_t)
 
 * 10. Add action to KeybindState::getAction(size_t)

 **** Enjoy your new Action that can be bound awesomely like a fucking thug thats right whats up


 WE ONLY USE RAPIDXML FOR:
  - Player::Player() -> to load in the saved keybindings in the config file
		(then OptionsState uses getAssignedKey() to retrieve the data and update the data)
  - KeybindState::finalizeBind(std::string) -> to save the new value in the config file

 */
