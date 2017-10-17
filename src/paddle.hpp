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

public:
	Paddle(float mX,float mY,bool updateStatus)
	{
		shape.setPosition(mX,mY);
		shape.setSize({defwidth,defheight});
		shape.setFillColor(defColor);
		shape.setOrigin(defwidth/2.f,defheight/2.f);
		updateRequired = updateStatus;
	}

	~Paddle()
	{}

	virtual void update() override
	{
		processPlayerInputs();
		shape.move(velocity);
	}

	void movePaddlePosition(float mX,float mY)
	{
		shape.setPosition(mX,mY);
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
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < wndWidth)
		    velocity.x = defVelocity;
        else
		    velocity.x = 0;
	}
};

const sf::Color Paddle::defColor{sf::Color::White};





#endif /* SRC_PADDLE_HPP_ */
