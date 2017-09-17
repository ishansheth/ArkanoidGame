/*
 * lives.hpp
 *
 *  Created on: Sep 9, 2017
 *      Author: ishan
 */

#ifndef SRC_LIVES_HPP_
#define SRC_LIVES_HPP_

#include "circle.hpp"
#include "entity.hpp"
#include <vector>

class lives : public Entity, public Circle
{
	sf::CircleShape shape;
public:
	static constexpr float defRadius{7.f};

	lives(float mX,float mY,bool updateStatus)
	{
		shape.setPosition(mX,mY);
		shape.setFillColor(sf::Color::White);
		shape.setRadius(defRadius);
		shape.setOrigin(defRadius,defRadius);
		updateRequired = updateStatus;
	}

	virtual void draw(sf::RenderWindow& mTarget) override
	{
			mTarget.draw(shape);
	}

	virtual void update() override
	{

	}

	virtual bool checkEntityDied() override
	{

	}
};




#endif /* SRC_LIVES_HPP_ */
