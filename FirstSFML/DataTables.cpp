#include "DataTables.h"

#include "Aircraft.h"
#include "Projectile.h"

std::vector<AircraftData> initAircraftData() {
	std::vector<AircraftData> data(Aircraft::TypeCount); //init's TypeCount spaces (since TypeCount is the last element of the Aircraft::Type enumerator, it creates the correct amount of spaces)
	
	//Player
	data[Aircraft::Type::Eagle].HP = 100;
	data[Aircraft::Type::Eagle].speed = 200.f;
	data[Aircraft::Type::Eagle].texture = Textures::ID::Eagle;
	data[Aircraft::Type::Eagle].fireInterval = sf::milliseconds(500);
	data[Aircraft::Type::Eagle].projectileSpread = 1;


	//Other planes - only these have "directions" (ie, AI).
	data[Aircraft::Type::Raptor].HP = 25;
	data[Aircraft::Type::Raptor].speed = 100.f;
	data[Aircraft::Type::Raptor].texture = Textures::ID::Raptor;
	data[Aircraft::Type::Raptor].fireInterval = sf::milliseconds(1500); //remember, if we use sf::Time::Zero, it means the enemy CANNOT fire.
	data[Aircraft::Type::Raptor].directions.push_back(Direction(45.f, 80.f));
	data[Aircraft::Type::Raptor].directions.push_back(Direction(-45.f, 160.f));
	data[Aircraft::Type::Raptor].directions.push_back(Direction(45.f, 80.f));
	data[Aircraft::Type::Raptor].projectileSpread = 3;

	data[Aircraft::Type::Avenger].HP = 50;
	data[Aircraft::Type::Avenger].speed = 50.f;
	data[Aircraft::Type::Avenger].texture = Textures::ID::Avenger;
	data[Aircraft::Type::Avenger].fireInterval = sf::seconds(2);
	data[Aircraft::Type::Avenger].directions.push_back(Direction(45.f, 50.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(0.f, 50.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(-45.f, 100.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(0.f, 50.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(45.f, 50.f));
	data[Aircraft::Type::Avenger].projectileSpread = 4;

	return data;
}


std::vector<ProjectileData> initProjectileData() {
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 500.f;
	data[Projectile::AlliedBullet].texture = Textures::Bullet;

	data[Projectile::EnemyBullet].damage = 20;
	data[Projectile::EnemyBullet].speed = 250.f;
	data[Projectile::EnemyBullet].texture = Textures::Bullet;

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 100.f;
	data[Projectile::Missile].texture = Textures::Missile;

	return data;
}

