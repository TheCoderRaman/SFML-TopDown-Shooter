#include "Entity.h"
#include <cassert>

Entity::Entity(int hitpoints)
: HP(hitpoints)
, velocity()
{

}

void Entity::setVelocity(sf::Vector2f velo) {
	velocity = velo;
}

void Entity::setVelocity(float vX, float vY) {
	velocity.x = vX;
	velocity.y = vY;
	//or; velocity = sf::Vector2f(vX, vY);
}

sf::Vector2f Entity::getVelocity() const {
	return velocity;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands) {
	move(velocity*dt.asSeconds()); // == setPosition(getPosition() + offset) where offset = (velocity*dt.asSeconds())
}

void Entity::repair(int amount) {
	assert(amount > 0);
	HP += amount;
}

void Entity::damage(int amount) {
	assert(amount > 0);
	HP = std::max(HP - amount, 0);
}

void Entity::destroy() {
	HP = 0;
}

int Entity::getHitpoints() const {
	return HP;
}

bool Entity::isDestroyed() const {
	return (HP == 0); //Won't be lower than 0, because damage limits HP to 0, and destroy sets HP to 0. therefore, HP == 0 means that it has been destroyed 
}

