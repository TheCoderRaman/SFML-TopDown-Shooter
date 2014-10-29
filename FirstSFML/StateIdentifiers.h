#ifndef STATE_IDENTIFIERS_H
#define STATE_IDENTIFIERS_H


namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
		Options,
		Settings,
		Game,
		Loading,
		Pause,
	};
}

#endif 



//To add a new state, make sure you add it's ID here,
//and MAKE SURE you call Application::mStateStack.registerState<NAMEState>(States::STATEID) on it
//in Application.cpp