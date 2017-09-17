/*
 * bullet.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: ishan
 */

#ifndef SRC_BULLET_HPP_
#define SRC_BULLET_HPP_

#include "entity.hpp"
#include "circle.hpp"

class Bullet : public Entity, public Circle
{
	static constexpr float defRadius{5.f};
	static constexpr float defVelocity{10.f};
	sf::CircleShape shape;
	sf::Vector2f velocity{0.f,-defVelocity};
public:
	bool isStruck{false};
	Bullet(float mX,float mY,bool updateStatus)
	{
		shape.setPosition(mX,mY);
		shape.setRadius(defRadius);
		shape.setFillColor(sf::Color::White);
		shape.setOrigin(defRadius,defRadius);
		updateRequired = updateStatus;
	}
	~Bullet(){}
	virtual void update() override{shape.move(velocity);}
	virtual bool checkEntityDied(){return ( shape.getPosition().y < 0 || isStruck);}
	virtual void draw(sf::RenderWindow& mTarget) override {mTarget.draw(shape);}
};
#endif /* SRC_BULLET_HPP_ */
