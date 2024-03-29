#ifndef SRC_PADDLE_HPP_
#define SRC_PADDLE_HPP_

#include "entity.hpp"
#include "rectangle.hpp"
#include "macros.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include "bullet.hpp"

/**
 * This class represents the paddle entity in the game. It takes care of its display and also its movement
 */
class Paddle : public Rectangle, public UpdatableEntity
{
public:
  static const sf::Color defColor;
  static constexpr float defheight{10.f};
  static constexpr float defwidth{100.f};
  static constexpr float defVelocity{8.f};

  static constexpr int MAX_BULLET = 1;
  
  sf::Vector2f velocity;
  sf::Vector2f lastPosition;
  
  int circularPathRadius{300};
  
  float angle;
  float rotationAngle;
  
  struct circleCoordinate
  {
    double x;
    double y;
    void setX(int a){x = a;}
    void setY(int b){y = b;}
  };

  std::vector<Bullet> bulletContainer;

  bool isBullet;
public:
  Paddle(float mX,float mY,bool updateStatus);
  
  ~Paddle();
  
  virtual void update() override;
  
  void movePaddlePosition(float mX,float mY);

  virtual void draw(sf::RenderWindow& window) override;

  void shootBullet();

  void disableBullet(); 

  std::vector<Bullet>& getBulletContainer(){return bulletContainer;}
private:
  void processPlayerInputs();

  void rotatePaddle();
  
};
#endif /* SRC_PADDLE_HPP_ */
