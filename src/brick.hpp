/*
 * brick.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: ishan
 */

#ifndef SRC_BRICK_HPP_
#define SRC_BRICK_HPP_

#include "entity.hpp"
#include "rectangle.hpp"

class Brick : public Rectangle,public Entity
{
public:
	sf::Color defColor;
	static constexpr float defHeight{20.f};
	static constexpr float defWidth{60.f};
	static constexpr float defVelocity{4.f};
	sf::Vector2f velocity{-defVelocity,0.f};
	int hitsRequired;
	int strength;

	Brick(float mX, float mY, sf::Color color,int hits,bool updateStatus)
	{
		defColor = color;
		shape.setPosition(mX,mY);
		shape.setSize({defWidth,defHeight});
		shape.setFillColor(defColor);
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(2.f);
		shape.setOrigin(defWidth/2.f,defHeight/2.f);
		hitsRequired = strength = hits;
		updateRequired = updateStatus;
	}

	~Brick()
	{

	}

	virtual void draw(sf::RenderWindow& window) override
	{
		window.draw(shape);
	}

	virtual void update() override
	{
//		handleBrickMovements();
//		shape.move(velocity);
	}

	virtual bool checkEntityDied() override
	{
		if(hitsRequired == 0)
		{
			return true;
		}else{
			return false;
		}
	}
private:
	void handleBrickMovements()
	{
		if(left()<0)
		{
			shape.setPosition(left()+wndWidth,y());
		}
	}
};
//const sf::Color Brick::defColor{sf::Color::Red};




#endif /* SRC_BRICK_HPP_ */
