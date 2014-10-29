#ifndef WORLD_H
#define WORLD_H

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Aircraft.h"
#include "CommandQueue.h"

#include <array>
/*
 * The World! Includes the view, scenegraph, textures
 * bounds and spawn positions, background scroll speeds etc.
 * While this class does contain an Aircraft (playerAircraft), it does NOT contain a Player
 * input controller class. This is information is contained where input is gathered, in Game.
 */
//Forward Declaration of RenderWindow
namespace sf {
	class RenderWindow;
}

class World : private sf::NonCopyable {
	public:
		explicit World(sf::RenderWindow& window, FontHolder& fonts);
		void update(sf::Time dt);
		void draw();
		CommandQueue& getCommandQueue();

	private:
		void loadTextures();
		void buildScene();
		void adaptPlayerVelocity();
		void adaptPlayerPosition();

		sf::FloatRect getViewBounds() const;
		sf::FloatRect getBattlefieldBounds() const;

		void spawnEnemies();
		void addEnemy(Aircraft::Type type, float relX, float relY);
		void addEnemies();

	private:
		enum Layer {
			Background, // == 0
			Air,		// == 1
			LayerCount  // == 2   -->Isn't that fucking smart? LayerCounts shows that there are 2 layers, even though index starts at 0
		};

		struct SpawnPoint {
			SpawnPoint(Aircraft::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
			{

			}
			Aircraft::Type type;
			float x;
			float y;
		};

	private:
		sf::RenderWindow&	mWindow;
		sf::View			mWorldView;
		TextureHolder		mTextures;
		FontHolder&			mFonts;
		SceneNode			mSceneGraph; //the ROOT node!

		//Array of SceneNode*, with LayerCount number of elements (ie, 2)
		std::array<SceneNode*, LayerCount> mSceneLayers;

		CommandQueue		mCommandQueue;
		sf::FloatRect		mWorldBounds;
		sf::Vector2f		mSpawnPosition;
		float				mScrollSpeed;

		Aircraft*			mPlayerAircraft; //the Player

		std::vector<SpawnPoint> mEnemySpawnPoints; //holds all future enemy spawn points.
												  //allows us to only create enemies when their
												  //position enters the viewable battlefield.


};

#endif