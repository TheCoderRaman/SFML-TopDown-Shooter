#include <SFML/Graphics/RenderWindow.hpp>

#include "World.h"
#include <algorithm>
#include <iostream>


/* PUBLIC FUNCTIONS */
World::World(sf::RenderWindow& window, FontHolder& fonts)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mFonts(fonts)
, mSceneGraph()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f) //50 pixels up per update
, mPlayerAircraft(nullptr)
, mCommandQueue() //each world has its ownm command queue, there should really only be 1 world though
{ 
	//mWindow.setFramerateLimit(60);
	loadTextures();
	buildScene();
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt) {
	// Scroll the world, reset player velocity
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

	spawnEnemies(); //constantly spawning enemies that are within range

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
}

void World::draw() {
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue() {
	return mCommandQueue;
}
/*END OF PUBLIC FUNCTIONS*/

/*PRIVATE FUNCTIONS*/
void World::loadTextures() {
	mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
	mTextures.load(Textures::Raptor, "Media/Textures/Raptor.png");
	mTextures.load(Textures::Avenger, "Media/Textures/Avenger.png");

	mTextures.load(Textures::Desert, "Media/Textures/Desert.png");

	mTextures.load(Textures::Bullet, "Media/Textures/Bullet.png");
	mTextures.load(Textures::Missile, "Media/Textures/Missile.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::PTR layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::Desert);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[World::Layer::Background]->attachChild(std::move(backgroundSprite));

	// Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures, mFonts));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	//mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
	mSceneLayers[World::Layer::Air]->attachChild(std::move(leader));

	addEnemies(); //adds enemy data to spawnpoints
}

void World::adaptPlayerVelocity() {
	sf::Vector2f vel = mPlayerAircraft->getVelocity();

	if (vel.x != 0.f && vel.y != 0.f)
		mPlayerAircraft->setVelocity(vel / std::sqrt(2.f));

	mPlayerAircraft->setVelocity(mPlayerAircraft->getVelocity() + sf::Vector2f(0.f, mScrollSpeed)); //ie, ->accelerate(0.f, mScrollSpeed);
}

void World::adaptPlayerPosition() {
	//keep player's pos inside screen bounds, or at least borderLimit (float data type defined below) away from border
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDist = 40.f;

	sf::Vector2f pos = mPlayerAircraft->getPosition();
	pos.x = std::max(pos.x, viewBounds.left + borderDist); //ensures we do not veer too far left. // viewBounds.left+borderDist is the LEAST it can be, so we ENSURE pos.x is always greater than the least it can be
	pos.y = std::max(pos.y, viewBounds.top + borderDist); //ensures we do not go above (ensures pos.y always greater than [ie, always below - because y-axis points down!] top border)
	pos.x = std::min(pos.x, viewBounds.left + viewBounds.width - borderDist); //ensure pos.x < RIGHT BOUND (ie, dont go too far right)
	pos.y = std::min(pos.y, viewBounds.top + viewBounds.height - borderDist); //ensure pos.y < BOTTOM BOUWN (ie, don't go too far bottom);
	mPlayerAircraft->setPosition(pos);
}

sf::FloatRect World::getViewBounds() const {
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize()); //unsure why this calculation works. possible wizardry. figure it out
}

sf::FloatRect World::getBattlefieldBounds() const {
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f; //Raises the top by 100px. Allows enemies to spawn off the top of the screen and move inwards (instead of just popping on top of screen)
	bounds.height += 100.f; //we also have to increase height by the same amount! :)
	return bounds; //Things now spawn off screen! 
}

void World::spawnEnemies() {
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top) //while there are remaining enemies that havent spawned, AND, while there are enemies withing battlefield view, and must be spawned

	{
		SpawnPoint spawn = mEnemySpawnPoints.back(); //LIFO
		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f); //so it faces downwards (ie, towards our player)

		mSceneLayers[World::Layer::Air]->attachChild(std::move(enemy));

		mEnemySpawnPoints.pop_back(); //we've spawned the enemy (which was taken from the end of the stack), so we simply pop to remove that shit
	}
}

void World::addEnemy(Aircraft::Type type, float relX, float relY) {
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY); //ENEMIES ADDED RELATIVELY TO THE PLAYERS STARTING POINT, which is center of view
	mEnemySpawnPoints.push_back(spawn);
}

void World::addEnemies() {
	addEnemy(Aircraft::Raptor, 0.f, 500.f);
	addEnemy(Aircraft::Raptor, 0.f, 1000.f);
	addEnemy(Aircraft::Raptor, +100.f, 1100.f);
	addEnemy(Aircraft::Raptor, -100.f, 1100.f);
	addEnemy(Aircraft::Avenger, -70.f, 1400.f);
	addEnemy(Aircraft::Avenger, -70.f, 1600.f);
	addEnemy(Aircraft::Avenger, 70.f, 1400.f);
	addEnemy(Aircraft::Avenger, 70.f, 1600.f);

	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), 
		[](SpawnPoint lhs, SpawnPoint rhs) {
			return lhs.y < rhs.y;
		}
	);
}