#include "SceneNode.h"
#include "Command.h"


#include <algorithm>
#include <cassert>

SceneNode::SceneNode() 
	: children(0)
	, parent(nullptr)
{

}


void SceneNode::attachChild(PTR child) {
	child->parent = this;
	children.push_back(std::move(child));
}

SceneNode::PTR SceneNode::detachChild(const SceneNode& node) {
	//Lambda expression -- checks all elements in container with O(n) complexity.
	//			-- stops if it finds the node and sets it to Found
	//			-- or reaches the end (ie, doesn't find it), and returns .end() (which is null)
	auto found = std::find_if(children.begin(), children.end(),
		[&](PTR& p) -> bool {return p.get() == &node; });

	assert(found != children.end());

	PTR res = std::move(*found); //moving a unique_ptr!
	res->parent = nullptr;
	children.erase(found); //erasing from children vector

	return res;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands) {
	updateCurrent(dt, commands);
	updateChildren(dt, commands); //recursively updates children (and any children it may have and so on) until all are drawn
}

//To be overloaded by the derived class Entity
void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands) {
	//do nothing by default - this is a layer node, we're interested in drawing its children
}
 
void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands) {
	for (auto itr = children.begin(); itr != children.end(); itr++) {
		(*itr)->update(dt, commands); //updates the current node (ie the child) and its children
	}
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//two transforms chained, one after the other.
	//Combines the parent's absolute transform with the current node's relative one
	//the result is the absolute transform of the CURRENT node 
	states.transform *= getTransform(); //now, states.transform contains the absolute world transform

	drawCurrent(target, states); //draw's the CURRENT node (ie the parent)
	drawChildren(target, states); //recursively calls draw until all the children nodes are drawn
}

//To be overloaded in the classes derived from Entity (ie, Planes, Enemys, HealthPickups, etc)
void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	//do nothing by default
	//we want drawCurrent() to be overridden in derived classes 
	//ie, Entity is such a class, and it inherits from SceneNode!

}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const {
	//recursively calls draw (and drawCurrent) on the children nodes (AND their children, and so on)
	for (auto itr = children.begin(); itr != children.end(); itr++) {
		(*itr)->draw(target, states);
	}
}

sf::Vector2f SceneNode::getWorldPosition() const {
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
	sf::Transform transform = sf::Transform::Identity; //the identity transformation, does not have effect on obj
	for (const SceneNode* node = this; node != nullptr; node = node->parent)
		transform = node->getTransform() * transform;
	return transform;
}

unsigned int SceneNode::getCategory() const {
	return Category::Scene; //Base scene node (ie, layers), are designated as SCENES!
}

void SceneNode::onCommand(const Command& command, sf::Time dt) {
	//NOTE: we use & NOT &&... & is a BITWISE & operator, whose logic can be seen here:
	//    11001110
	//  & 10011000
	//	= 10001000
	//So, if there are any two matching 1 bits in LHS and RHS, the result will be non-0, and the if conditional will pass
	//so basically: If the command is issued to anyone, execute that command for those it is issued to.

	if (command.category & getCategory())
		command.action(*this, dt);

	for (auto itr = children.begin(); itr != children.end(); itr++) {
		(*itr)->onCommand(command, dt); //updates commands to rest of the scene node children
	}
}

sf::FloatRect SceneNode::getBoundingRect() const {
	return sf::FloatRect();
}
