#include "ParallelTask.h"

ParallelTask::ParallelTask()
	: thread(&ParallelTask::runTask, this)
	, finished(false)
{

}

void ParallelTask::execute() {
	finished = false;
	elapsedTime.restart();
	thread.launch(); //launches the function linked in the ParallelTask ctor (ie, : thread(&ParallelTask::runTask(), ...) )
}

//LoadingState::update() accesses this function. Notice how we lock it
// because LoadingState::update() is the main thread, and ParallelTask::finished
//can be changed by the task thread, we need to lock sensitive data before we use it.
//By locking the Mutex object before touching sensitive data, we ensure that if a thread 
//tries to lock an already locked resource, it will wait until the other thread unlocks it.
//sf::Lock is RAII compliant, ie, it will expire as soon as scope does, and is destructed
//unlocking the mutex object automatically!
bool ParallelTask::isFinished() {
	sf::Lock lock(mutex); //the mutex passed to lock is automatically locked
	return finished; 
}

float ParallelTask::getCompletion() {
	sf::Lock lock(mutex);
	//100% at 10 secs of elapsed time
	return elapsedTime.getElapsedTime().asSeconds() / 3.f;
}

//here's the shit we want to do in parallel!
//load textures and fonts here! that is your task, young grasshopper
//also stop talking to yourself in the comments.
void ParallelTask::runTask() {
	bool ended = false;
	while (!ended) {
		sf::Lock lock(mutex); //protects the clock?????
		if (elapsedTime.getElapsedTime().asSeconds() >= 3.f)
			ended = true;
	}
	//extra brackets are  way or releasing that same lock as soon as possible
	{//finished may be accessed from multiple threads, protect it
		sf::Lock lock(mutex);
		finished = true;
	}
}