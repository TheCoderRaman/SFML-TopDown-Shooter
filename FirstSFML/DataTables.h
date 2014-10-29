#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "ResourceIdentifiers.h"
#include <vector>

#include <SFML/System/Time.hpp>

struct Direction {
	Direction(float angle, float distance)
		: angle(angle)
		, distance(distance)
	{

	}
	float angle;
	float distance;
};

struct AircraftData {
	int HP;
	float speed;
	Textures::ID texture;
	sf::Time fireInterval;
	int projectileSpread;

	std::vector<Direction> directions;
};

struct ProjectileData {
	int damage;
	float speed;
	Textures::ID texture;
};



std::vector<AircraftData> initAircraftData();
std::vector<ProjectileData> initProjectileData();
#endif