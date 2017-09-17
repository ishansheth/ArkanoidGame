/*
 * ball.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: ishan
 */

#ifndef SRC_BALL_HPP_
#define SRC_BALL_HPP_

#include "entity.hpp"
#include "circle.hpp"

constexpr unsigned int wndWidth{800},wndHeight{600};

class Ball : public Circle, public Entity
{
public:
	static const sf::Color defColor;
	static constexpr float defRadius{5.f};
	static constexpr float defVelocity{2.f};
	sf::Vector2f velocity{-defVelocity,-defVelocity};

//	bool isLeftCrossed(){ return((x() - shape.getRadius()) < 0);}
//	bool isRightCrossed(){return((x() + shape.getRadius()) > wndWidth);}
//	bool isTopCrossed(){return((y() - shape.getRadius()) < 0);}
//	bool isBottomCrossed(){return((y() + shape.getRadius()) > wndHeight);}

	Ball(float mX,float mY,bool updateStatus)
	{
		shape.setPosition(mX,mY);
		shape.setRadius(defRadius);
		shape.setFillColor(defColor);
		shape.setOrigin(defRadius,defRadius);
		updateRequired = updateStatus;
	}

	~Ball()
	{}

	sf::Vector2f setVelocity(float x, float y)
	{
		velocity.x = x;
		velocity.y = y;
	}

	void setPosition(float mX,float mY)
	{
		shape.setPosition(mX,mY);
	}

	virtual void update() override
	{
		solveBoundCollisions();
		shape.move(velocity);
	}

	virtual void draw(sf::RenderWindow& mTarget) override
	{
		mTarget.draw(shape);
	}

	virtual bool checkEntityDied() override
	{
		return(bottom() > wndHeight);
	}

	void solveBallPaddleRelativeMotion()
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
		{
			velocity.x = -8.f;
			velocity.y = 0;
		}
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < wndWidth)
        {
		    velocity.x = 8.f;
        }
        else
        {
		    velocity.x = 0;
			velocity.y = 0;
        }
		shape.move(velocity);

	}

private:
	void solveBoundCollisions() noexcept
	{
        if(left() < 0)
            velocity.x = -velocity.x;
        else if(right() > wndWidth)
            velocity.x = -velocity.x;

        if(top() < 0)
            velocity.y = -velocity.y;
        else if(bottom() > wndHeight)
            velocity.y = -velocity.y;

//		if(isLeftCrossed() || isRightCrossed())
//		{
//			velocity.x = -velocity.x;
//		}
//		else if(isTopCrossed() || isBottomCrossed())
//		{
//			velocity.y = -velocity.y;
//		}

	}

};

const sf::Color Ball::defColor{sf::Color::Green};




#endif /* SRC_BALL_HPP_ */
