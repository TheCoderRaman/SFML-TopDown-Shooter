#include "Projectile.h"
#include "DataTables.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace { //anonymous namespace - avoids name collisions in other files, since we will repeatedly be using similar data tables.
	const std::vector<ProjectileData> Table = initProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures) 
	: Entity(1) //1 HP - so it's not detected as destroyed and not drawn
	, type(type)
	, sprite(textures.get(Table[type].texture))
{
	centerOrigin(sprite);
}

/*PUBLIC FUNCTIONS*/
void Projectile::guideTowards(sf::Vector2f position) {
	assert(isGuided());
	targetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const {
	return (type == Missile);
}

unsigned int Projectile::getCategory() const {
	if (type == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}

//Retrieves the bounding rectangle of sprite of the projectile with all its current transforms (rotations, scales, etc)
sf::FloatRect Projectile::getBoundingRect() const {
	return getWorldTransform().transformRect(sprite.getGlobalBounds()); 
}

float Projectile::getMaxSpeed() const {
	return Table[type].speed;
}

int Projectile::getDamage() const {
	return Table[type].damage;
}
/*END OF PUBLIC FUNCTIONS*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*PRIVATE FUNCTIONS*/
void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands) {
	if (isGuided()) {
		const float approachRate = 200.f;

		sf::Vector2f newVel = unitVector(approachRate * dt.asSeconds() * targetDirection + getVelocity());
		newVel *= getMaxSpeed();
		float angle = std::atan2(newVel.y, newVel.x);

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVel); //rotation and velocity set
	}

	Entity::updateCurrent(dt, commands); //rotation and velocity drawn on screen
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
/*END OF PRIVATE FUNCTIONS*/
