#include "Aircraft.h"
#include "DataTables.h"
#include "Utility.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace { //anonymous namespace - avoids name collisions in other files, since we will repeatedly be using similar data tables.
	const std::vector<AircraftData> Table = initAircraftData(); //Table HAS to be initiated before the constructor starts, since it pulls data from "Table"
}

Aircraft::Aircraft(Type t, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[t].HP)
	, type(t)
	, sprite(textures.get(Table[t].texture))
	, healthDisplay(nullptr)
	, missileDisplay(nullptr)
	, fireCommand()
	, missileCommand()
	, fireRateCountdown(sf::Time::Zero)
	, isFiring(false)
	, isLaunchingMissile(false)
	, isMarkedForRemoval(false)
	, fireRate(1)
	, spreadLevel(Table[t].projectileSpread)
	, missileAmmo(2)
	, travelledDistance(0.f)
	, directionIndex(0)
{	
	centerOrigin(sprite);

	fireCommand.category = Category::Scene;
	fireCommand.action = [this, &textures](SceneNode& node, sf::Time) {
		createBullets(node, textures);
	};

	missileCommand.category = Category::Scene;
	missileCommand.action = [this, &textures](SceneNode& node, sf::Time) {
		createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
	};

	//Creating health text object for Aircraft
	std::unique_ptr<TextNode> hpDisplay(new TextNode(fonts, "")); //the actual object
	healthDisplay = hpDisplay.get(); //healthDisplay will be a pointer to hpDisplay
	attachChild(std::move(hpDisplay));

	if (getCategory() == Category::PlayerAircraft) {
		std::unique_ptr<TextNode> _missileDisplay(new TextNode(fonts, ""));
		_missileDisplay->setPosition(0, 70); //relative to the aircraft!
		missileDisplay = _missileDisplay.get();
		attachChild(std::move(_missileDisplay));
	}

	updateText(); //must also be called in the main update loop
}

/*Public functions*/
void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

unsigned int Aircraft::getCategory() const {
	switch (type) { //This right here can change a lot of behaviour, pay attention to WHAT we're returning
		case Aircraft::Eagle:
			return Category::PlayerAircraft; //no break needed cause of return
		case Aircraft::Raptor:
			return Category::AlliedAircraft;
		default:
			return Category::EnemyAircraft;
	}
}

float Aircraft::getMaxSpeed() const {
	return Table[type].speed;
}

bool Aircraft::isAllied() const {
	return type == Eagle; //cause this is the only allied dude apparently.
}

void Aircraft::fire() {
	//ship must have > 0 fire interval to fire anything
	if (Table[type].fireInterval != sf::Time::Zero)
		isFiring = true;
}

void Aircraft::launchMissile() {
	if (missileAmmo > 0) {
		isLaunchingMissile = true;
		missileAmmo--;
	}
}
/*End of Public functions*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Private functions */

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands) {
	if (isDestroyed()) {
		//checkPickupDrop(commands);

		isMarkedForRemoval = true;
		return;
	}

	checkProjectileLaunch(dt, commands);

	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands); //actually moves the object according to the velocity as Set in updateMovementPattern (setVelocity (vx, vy));
	
	updateText();
}

void Aircraft::updateMovementPattern(sf::Time dt) {
	const std::vector<Direction>& directions = Table[type].directions;

	if (!directions.empty()) {
		float distanceToTravel = directions[directionIndex].distance;
		if (travelledDistance > distanceToTravel) { //we've now finished with this directive
			directionIndex = (directionIndex + 1) % directions.size(); //we increment directionIndex (modulus allows a cycle - finishing last direction resets it to first direction)
			travelledDistance = 0.f; //reset distance travelled - and now we can happily await our next directive and let it follow orders
		}

		//compute's velocity from direction - ANGLE STUFF IS HERE!
		float radians = toRadian(directions[directionIndex].angle + 90.f); //first we turn angle by 90deg
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);
		setVelocity(vx, vy);
		travelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}


void Aircraft::updateText() {
	healthDisplay->setString(toString(getHitpoints()) + " HP"); //Always updates to current HP
	healthDisplay->setPosition(0.f, 50.f); //position relative to aircraft
	healthDisplay->setRotation(-getRotation()); //negative because it compensates the rotation in total - leaves text always upright. 

	if (missileDisplay) { //checks if ptr is not null (this will only be true for the Player aircraft, since we only init missileDisplay for the Player aircraft)
		if (missileAmmo == 0)
			missileDisplay->setString("");
		else
			missileDisplay->setString("M: "+toString(missileAmmo));
	}
}

//The reason this isn't done in projectile, is because
void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands) {
	if (!isAllied())
		fire(); //causes enmemies to fire the entire time

	//checks for automatic firing - but only allows the actual projectile through in intervals
	if (isFiring && fireRateCountdown <= sf::Time::Zero) {
		//interval has expired, we can fire a new bullet!
		commands.push(fireCommand);
		fireRateCountdown += Table[type].fireInterval / (fireRate + 1.f);
		isFiring = false;
	}
	else if (fireRateCountdown > sf::Time::Zero) {
		//interval hasn't expired yet, continue to decrease
		fireRateCountdown -= dt;
		isFiring = false;
	}

	//checks for missile launch
	if (isLaunchingMissile) {
		commands.push(missileCommand);
		isLaunchingMissile = false;
	}
}

void Aircraft::createBullets(SceneNode& node, const TextureHolder& textures) const {
	Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch (spreadLevel)
	{
		case 1:
			createProjectile(node, type, 0.0f, 0.5f, textures);
			break;

		case 2:
			createProjectile(node, type, -0.33f, 0.33f, textures);
			createProjectile(node, type, +0.33f, 0.33f, textures);
			break;

		case 3:
			createProjectile(node, type, -0.5f, 0.33f, textures);
			createProjectile(node, type, 0.0f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.33f, textures);
			break;

		case 4:
			createProjectile(node, type, -0.425f, 0.05f, textures);
			createProjectile(node, type, -0.3f, 0.05f, textures);
			createProjectile(node, type, 0.3f, 0.05f, textures);
			createProjectile(node, type, 0.425f, 0.05f, textures);
	}
}

void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const {

	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * sprite.getGlobalBounds().width, yOffset * sprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	projectile->setRotation(-getRotation());
	node.attachChild(std::move(projectile));
}
/*End of Private functions*/