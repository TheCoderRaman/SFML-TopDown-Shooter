//The base class of all game objects!
#ifndef ENTITY_H
#define ENTITY_H

#include "SceneNode.h"

class Entity : public SceneNode {
	public:
		explicit Entity(int hitpoints);
		void repair(int amount);
		void damage(int amount);
		void destroy();

		int getHitpoints() const;
		bool isDestroyed() const;

		void setVelocity(sf::Vector2f velo);
		void setVelocity(float vX, float vY);
		sf::Vector2f getVelocity() const;

	protected:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

	private:
		int HP; //hitpoints of an entity
		//sf::Vector2f default constructor initializes a zero vector (ie, (0.f, 0.f))
		sf::Vector2f velocity; 
};

#endif