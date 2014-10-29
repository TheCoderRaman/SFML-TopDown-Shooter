#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

/*
 * The Command queue - simply provides an interface for std::queue and some operations
 * which will have some further functionality (than the stdlib)
 */

#include "Command.h"
#include <queue>
class CommandQueue{

public:
	void	push(const Command& command);
	Command	pop();
	bool isEmpty() const;

private:
	std::queue<Command> queue;
};

#endif