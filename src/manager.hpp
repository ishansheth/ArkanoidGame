#ifndef SRC_MANAGER_HPP_
#define SRC_MANAGER_HPP_

#include "entity.hpp"
#include "ball.hpp"
#include "stageManager.hpp"
#include "utility.hpp"
#include <map>
#include <vector>
#include <memory>

/**
 * Manager class which manages all the entities in the game. Creating, destroying
 */
class Manager
{
private:

	std::vector<std::shared_ptr<Entity>> entities; // the vector which contains the entities object
	std::map< std::size_t,std::vector<Entity*> > groupedEntities; // the map which contains the entities object and can be accessed by the hash value of the object as a key

public:

	//Total number of lives the player has in the game
	int totalLives{3};

	Manager(){}

	/**
	 * a function templated with variadic arguments which is required to generate different kind of entities in the game with varying number of arguments
	 */
	template<typename T,typename...Args>
	T& create(Args&&...args)
	{
		static_assert(std::is_base_of<Entity,T>::value,"T must be derived from the Entity interface");
        auto uPtr(std::make_shared<T>(std::forward<Args>(args)...));
        auto ptr(uPtr.get());
        groupedEntities[typeid(T).hash_code()].emplace_back(ptr);
        entities.emplace_back(uPtr);
        return *ptr;
	}

	/**
	 * The function to remove all the entites from the map and vector in which the destroyed variable is set to true
	 */
	void refresh()
	{
		for(auto& pair: groupedEntities)
		{
			auto& vector(pair.second);
			vector.erase(std::remove_if(std::begin(vector),std::end(vector),
					[](Entity* ment)
					{
						return (ment->destroyed);
					}),std::end(vector));
		}

		entities.erase(std::remove_if(std::begin(entities),std::end(entities),
				[](std::shared_ptr<Entity> entSharedPtr)
				{
					return (entSharedPtr->destroyed);
				}),std::end(entities));
	}

	// clear the containers of the entities
	void clear()
	{
		groupedEntities.clear();
		entities.clear();
	}

	// The utility function which can be used to get the certain kind of entity from the map
	template<typename T>
	std::vector<Entity*> getAll()
	{
		return groupedEntities[typeid(T).hash_code()];
	}

	// The utility function which takes the entity type and functor as a template argument and calls the function with that entity as a parameter
	template<typename T, typename TFunc>
	void forEach(const TFunc& mFunc)
	{
		auto vector(getAll<T>());
		for(const auto& entity: vector)
		{
			mFunc(*reinterpret_cast<T*>(entity));
		}
	}

	template<typename T = Ball>
	bool checkBallDropped()
	{
		auto vector(getAll<T>());
		if(vector.size() == 1)
		{
			return vector[0]->checkEntityDied();
		}else{
			return false;
		}
	}

	template<typename T>
	T* getSingleEntity()
	{
		auto vector(getAll<T>());
		return static_cast<T*>(vector[0]);
	}

	void handleBallDrop()
	{
		Ball* ballentity = getSingleEntity<Ball>();
		ballentity->destroyed = true;

		auto vector(getAll<lives>());
		auto vectorIterator = vector.rbegin();
		for(;vectorIterator != vector.rend();++vectorIterator)
		{
			if((*vectorIterator)->destroyed)
			{
				continue;
			}else{
				(*vectorIterator)->destroyed = true;
				break;
			}
		}
	}
	void update()
	{
		for(const auto& e: entities)
		{
			if(!e->updateRequired)
			{
				e->update();
			}
		}
	}
	void draw(sf::RenderWindow& mTarget)
	{
		for(auto& e: entities)
		{
			e->draw(mTarget);
		}
	}
};

#endif /* SRC_MANAGER_HPP_ */
