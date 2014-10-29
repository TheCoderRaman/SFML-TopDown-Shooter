#ifndef SCENE_NODE_H
#define SCENE_NODE_H

/*
 * The SceneNode! A virtual representation of every entity
 * in the Game. This base class defines operations common to
 * SceneNodes, but also allows certain functions to be overridden.
 * By itself, this class will be used to represent the different 
 * SceneLayers in the SceneGraph (in World obj).
 */

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include "Category.h"

#include <vector>
#include <memory>

struct Command;
class CommandQueue;

class SceneNode 
	: public sf::Drawable
	, public sf::Transformable
	, public sf::NonCopyable 
{
	public:
		typedef std::unique_ptr<SceneNode> PTR;
	public:
		SceneNode();
						void	update(sf::Time dt, CommandQueue& commands);
		//Adds a child node to the Scene Node.
						void	attachChild(PTR child);
		//Removes the specified child Scene Node from this Scene Node (ie, the parent).
						 PTR	detachChild(const SceneNode& node);
		//Returns the absolute position of the SceneNode in the world.
				sf::Vector2f	getWorldPosition() const;
		//Takes into account all the parent transforms
			   sf::Transform	getWorldTransform() const;

						void	onCommand(const Command& cmd, sf::Time dt);
		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;

	private:
		//Update ops
		virtual void	updateCurrent(sf::Time dt, CommandQueue& commands);
				void	updateChildren(sf::Time dt, CommandQueue& commands);

		//Draw ops
		virtual void	draw(sf::RenderTarget& target,
							sf::RenderStates states) const;

		virtual void	drawCurrent(sf::RenderTarget& target,
							sf::RenderStates states) const;

				void	drawChildren(sf::RenderTarget& target,
							sf::RenderStates states) const;

	private:
		std::vector<PTR> children; //vector of unique_ptr<SceneNode> indicating children
			  SceneNode* parent; //pointer to the SOLE parent    
};

#endif