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
class Brick : public Rectangle,public RemovableEntity
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
  Brick(float mX, float mY, sf::Color color,int hits,bool updateStatus);

  ~Brick();

  virtual void draw(sf::RenderWindow& window) override;
 
  virtual void update() override;

  virtual void displayEntity(sf::Window& window);

  virtual bool checkEntityDied() override;

  bool isFlying();

  void flingBrick();
  
private:
  void handleBrickMovements();
};

//const sf::Color Brick::defColor{sf::Color::Red};
#endif /* SRC_BRICK_HPP_ */
