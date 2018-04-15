#ifndef SRC_BRICK_HPP_
#define SRC_BRICK_HPP_

#include "entity.hpp"
#include "rectangle.hpp"
#include "macros.hpp"
#include <thread>
#include <chrono>

/**
 * This class shows the bricks entities and it is responsible to display the bricks on the window
 */
class Brick : public Rectangle,public Entity
{
	bool flingFlag{false};
	sf::Texture brickTexture;
public:
	sf::Color defColor;
	static constexpr float defHeight{BRICKHEIGHT};
	static constexpr float defWidth{BRICKWIDTH};
	static constexpr float defVelocity{4.f};
	sf::Vector2f velocity{-defVelocity,0.f};
	sf::Vector2f velocityFlig{-defVelocity,-defVelocity};

	// No of hits still required to destroy the brick. If that reaches the strength, then it will break
	int hitsRequired;
	int strength;

	// The creation of the brick object requires the location, color, no of hits required, current stage count of the game and it it has to be updated or not
	// Perhaps, no of hits and current status can remove the need of updatestatus variable
	Brick(float mX, float mY, sf::Color color,int hits,int currentstagecount,bool updateStatus)
	{
		defColor = color;
		shape.setPosition(mX,mY);
		shape.setSize({defWidth,defHeight});
		shape.setFillColor(defColor);
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(2.f);
		shape.setOrigin(defWidth/2.f,defHeight/2.f);
		brickTexture.loadFromFile(STRINGIZE_VALUE_OF(BRICKTEXTUREIMAGEFILE));
		shape.setTexture(&brickTexture,false);
		hitsRequired = strength = hits;
		updateRequired = updateStatus;
		stagecount = currentstagecount;
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
		// if the game is in second stage onwards, then bricks will move
		if(flingFlag)
		{
			shape.rotate(10);
			shape.move(velocityFlig);
		}
		if(x()<0)
		{
			destroyed = true;
		}
	}

	virtual void displayEntity(sf::Window& window)
	{
		window.setActive(true);
		window.display();
	}


	virtual bool checkEntityDied() override
	{
		return (hitsRequired == 0);
	}

	bool isFlying(){return flingFlag;}

	void flingBrick()
	{
		flingFlag = true;
	}

private:
	void handleBrickMovements()
	{
		if(left()<0)
		{
			shape.setPosition(left()+WNDWIDTH,y());
		}
	}
};

//const sf::Color Brick::defColor{sf::Color::Red};
#endif /* SRC_BRICK_HPP_ */
