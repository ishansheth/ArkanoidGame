#ifndef SRC_PADDLE_HPP_
#define SRC_PADDLE_HPP_

#include "entity.hpp"
#include "rectangle.hpp"

/**
 * This class represents the paddle entity in the game. It takes care of its display and also its movement
 */
class Paddle : public Rectangle, public Entity
{
public:
	static const sf::Color defColor;
	static constexpr float defheight{10.f};
	static constexpr float defwidth{100.f};
	static constexpr float defVelocity{8.f};

	sf::Vector2f velocity;
	sf::Vector2f lastPosition;

public:
	Paddle(float mX,float mY,bool updateStatus):angle(0)
	{
		shape.setPosition(mX,mY);
		lastPosition = {mX,mY};
		shape.setSize({defwidth,defheight});
		shape.setFillColor(defColor);
		shape.setOrigin(defwidth/2.f,defheight/2.f);
		updateRequired = updateStatus;
	}

	~Paddle(){}

	virtual void update() override
	{
		processPlayerInputs();
		shape.move(velocity);
	}

	void movePaddlePosition(float mX,float mY)
	{

		if(lastPosition.x != mX)
		{
			if(lastPosition.x < mX)
			{
				sf::Vector2f vel{1,0};
				while(abs(shape.getPosition().x-mX) != 0)
				{
					// this delay is necessary to make movement of paddle observable
					std::this_thread::sleep_for (std::chrono::microseconds(800));
					shape.move(vel);
				}
				lastPosition = {mX,mY};
			}else{
				sf::Vector2f vel{-1,0};
				while(abs(shape.getPosition().x-mX) != 0)
				{
					// this delay is necessary to make movement of paddle observable
					std::this_thread::sleep_for (std::chrono::microseconds(800));
					shape.move(vel);
				}
				lastPosition = {mX,mY};
			}
		}
	}

	virtual void draw(sf::RenderWindow& window) override {window.draw(shape);}

	virtual bool checkEntityDied() override
	{
		return false;
	}

private:
	void processPlayerInputs()
	{

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
			velocity.x = -defVelocity;
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < WNDWIDTH)
		    velocity.x = defVelocity;
        else
		    velocity.x = 0;
	}
};
const sf::Color Paddle::defColor{sf::Color::White};
#endif /* SRC_PADDLE_HPP_ */
