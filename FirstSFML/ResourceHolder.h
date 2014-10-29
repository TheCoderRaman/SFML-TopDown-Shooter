#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

//This is a template class for a ResourceHolder object.
//Instead of defining identical Holders for different SFML objects
//such as Textures, Sprites, Images (not Music - doesn't follow .loadFromX() SFML convenction
// it utilizes .openFromX()) , etc - we define a common class that provides
//common functionality between all of them. 
//It takes 2 typename parameters:
//Resource - the Resource we'd like to point to (ie, the texture)
//Identifier - the identifier (preferrably an enum ID {...})
//The core data member is the resource_map, which is a:
//std::map<Identifier, std::unique_ptr<Resource>> designed in a 
//KEY-VALUE manner. Thus, we can .get(ID) to return the corresponding
//resource!
template <typename Resource, typename Identifier>
class ResourceHolder
{
	public:
		void						load(Identifier id, const std::string& filename);

		template <typename Parameter>
		void						load(Identifier id, const std::string& filename, const Parameter& secondParam);

		Resource&					get(Identifier id);
		const Resource&				get(Identifier id) const;

	private:
		void						insertResource(Identifier id, std::unique_ptr<Resource> resource);


	private:
		std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
};

//this is important for templated classes. The compiler needs
//to know about the rest of the functions, so we include the cpp file
#include "ResourceHolder.inl"

#endif