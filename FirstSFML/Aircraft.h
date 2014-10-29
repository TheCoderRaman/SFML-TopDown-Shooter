#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "TextNode.h"
#include "Entity.h"
#include "ResourceHolder.h" //get rid of this? maybe move to resource ident? eeeh. efficiency, yo.
#include "ResourceIdentifiers.h"

#include "Projectile.h"

#include "CommandQueue.h"

#include <SFML/Graphics/Sprite.hpp>

class Aircraft : public Entity {
	public:
		enum Type {
			Eagle, 
			Raptor,
			Avenger,
			TypeCount
		};

	public:
		Aircraft(Type t, const TextureHolder& textures, const FontHolder& fonts);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual unsigned int getCategory() const;
		float getMaxSpeed() const;

		bool isAllied() const;

		void fire();
		void launchMissile();

	private:
		virtual void updateCurrent(sf::Time dt, CommandQueue& cmds);
		void updateMovementPattern(sf::Time dt);
		void updateText();

		void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

		void createBullets(SceneNode& node, const TextureHolder& textures) const;
		void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

	private:
		Type type;
		sf::Sprite sprite;

		Command fireCommand;
		Command missileCommand;
		sf::Time fireRateCountdown;
		bool isFiring;
		bool isLaunchingMissile;
		bool isMarkedForRemoval;

		int fireRate;
		int spreadLevel;
		int missileAmmo;


		TextNode*	healthDisplay;
		TextNode*	missileDisplay;

		float travelledDistance;
		std::size_t directionIndex; //keeps track of which part of the direction list we're on
									//ie, an Avenger has 5 directives
									//and directionIndex will tell us
									//which the plane is currently doing.
};

#endif
