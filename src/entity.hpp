/*
 * entity.hpp
 *
 *  Created on: Sep 4, 2017
 *      Author: ishan
 */

#ifndef SRC_ENTITY_HPP_
#define SRC_ENTITY_HPP_

class Entity
{
public:
	bool destroyed{false};
	bool updateRequired{false};

	virtual ~Entity()=0;
	virtual void update()=0;
	virtual bool checkEntityDied()=0;
	virtual void draw(sf::RenderWindow & window)=0;

};

Entity::~Entity()
{}



#endif /* SRC_ENTITY_HPP_ */
