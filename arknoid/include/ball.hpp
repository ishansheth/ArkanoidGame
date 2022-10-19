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


class Ball : public Circle, public RemovableEntity
{
  ParticleSystem particles;
  bool isParticles;
  sf::Clock clock;

public:
  // TODO: build better interface, private and public variables separation
  static const sf::Color defColor;
  static constexpr float defRadius{5.f};
  sf::Vector2f velocity;
  std::shared_ptr<SoundEntity> boosterSound;
  std::shared_ptr<SoundEntity> beepSound;

  
  Ball(float mX,float mY,bool updateStatus,float xvector,float yvector);
  
  ~Ball();
  
  sf::Vector2f getVelocity();

  void applyParticles();

  void disableParticles();
  
  void setPosition(float mX,float mY);
  
  sf::Vector2f getPosition();
  
  virtual void update() override;
  
  virtual void draw(sf::RenderWindow& mTarget) override;
  
  virtual bool checkEntityDied() override;
  
  /**
   * This function takes care of moving the ball along with paddle when its sitting on the paddle.
   * So it creates the relative motion between ball and paddle
   */
  void solveBallPaddleRelativeMotion();
  
private:
  // This function keeps the ball inside the window and does not let it go out
  void solveBoundCollisions() noexcept;
};
#endif /* SRC_BALL_HPP_ */
