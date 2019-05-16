#ifndef SRC_BALL_HPP_
#define SRC_BALL_HPP_

#include "entity.hpp"
#include "circle.hpp"
#include "SoundEntity.hpp"
#include "BallSound.hpp"
#include "BoosterSound.hpp"
#include "macros.hpp"
#include <memory>
#include "particlesystem.hpp"

/**
 * Ball entity which is resonsible for the ball in the game.
 * This class takes care of the circle shape, its properties and its movement
 */


class Ball : public Circle, public Entity
{
public:
  static const sf::Color defColor;
  static constexpr float defRadius{5.f};
  sf::Vector2f velocity;
  std::shared_ptr<SoundEntity> beepSound;
  std::shared_ptr<SoundEntity> boosterSound;
  ParticleSystem particles;
  bool isParticles;
  sf::Clock clock;

  
  Ball(float mX,float mY,bool updateStatus,float xvector,float yvector):particles(2000),isParticles(false),clock()
	{
		velocity.x = xvector;
		velocity.y = yvector;
		shape.setPosition(mX,mY);
		shape.setRadius(defRadius);
		shape.setFillColor(defColor);
		shape.setOrigin(defRadius,defRadius);
		updateRequired = updateStatus;
		beepSound = std::make_shared<BallSound>(STRINGIZE_VALUE_OF(BEEPSOUNDFILE));
		boosterSound = std::make_shared<BoosterSound>(STRINGIZE_VALUE_OF(BOOSTERSOUNDFILE));
		particles.setEmitter(sf::Vector2f(WNDHEIGHT/2,WNDWIDTH/2));

	}
  
  ~Ball(){}
  
  sf::Vector2f getVelocity(){ return velocity; }

  void applyParticles(){
    if(!isParticles){
      velocity.x *= 5;
      velocity.y *= 5;
      isParticles = true;
    }
  }

  void disableParticles(){
    if(isParticles){
      velocity.x /= 5;
      velocity.y /= 5;
      isParticles = false;
      boosterSound->pauseSound();
    }
  }
  
  void setPosition(float mX,float mY)
  {
    shape.setPosition(mX,mY);
  }
  
  sf::Vector2f getPosition()
  {
    return shape.getPosition();
  }
  
  virtual void update() override
  {
    
    sf::Time elapsed = clock.restart();
    particles.update(elapsed);
    particles.setEmitter(shape.getPosition());
    solveBoundCollisions();
    shape.move(velocity);
    if(isParticles){
      boosterSound->playSound();
    }
  }
  
  virtual void draw(sf::RenderWindow& mTarget) override
  {
    mTarget.draw(shape);
    if(isParticles)
      mTarget.draw(particles);
  }
  
  virtual bool checkEntityDied() override
  {
    return(bottom() > WNDHEIGHT);
  }
  
  /**
   * This function takes care of moving the ball along with paddle when its sitting on the paddle.
   * So it creates the relative motion between ball and paddle
   */
  void solveBallPaddleRelativeMotion()
  {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
      {
	velocity.x = -8.f;
	velocity.y = 0;
      }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < WNDWIDTH)
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
  // This function keeps the ball inside the window and does not let it go out
  void solveBoundCollisions() noexcept
  {
    
    if(left() < 0)
      {
	beepSound->playSound();
	velocity.x = -velocity.x;
      }
    else if(right() > WNDWIDTH)
      {
	beepSound->playSound();
	velocity.x = -velocity.x;
      }
    
    if(top() < 0)
      {
	velocity.y = -velocity.y;
	beepSound->playSound();
      }
    else if(bottom() > WNDHEIGHT)
      {
	velocity.y = -velocity.y;
	beepSound->playSound();
      }
    
  }
};
// Green color of the ball is set fixed as a static variable
const sf::Color Ball::defColor{sf::Color::Green};
#endif /* SRC_BALL_HPP_ */
