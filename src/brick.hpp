#ifndef SRC_BRICK_HPP_
#define SRC_BRICK_HPP_

#include "entity.hpp"
#include "rectangle.hpp"

/**
 * This class shows the bricks entities and it is responsible to display the bricks on the window
 */
class Brick : public Rectangle,public Entity
{
public:
	sf::Color defColor;
	static constexpr float defHeight{20.f};
	static constexpr float defWidth{60.f};
	static constexpr float defVelocity{4.f};
	sf::Vector2f velocity{-defVelocity,0.f};

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
		hitsRequired = strength = hits;
		updateRequired = updateStatus;
		stagecount = currentstagecount;
	}

	~Brick(){}

	virtual void draw(sf::RenderWindow& window) override
	{
		window.draw(shape);
	}

	virtual void update() override
	{
		// if the game is in second stage onwards, then bricks will move
		if(stagecount > 1)
		{
			handleBrickMovements();
			shape.move(velocity);
		}
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
