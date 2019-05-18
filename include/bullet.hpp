#ifndef SRC_BULLET_HPP_
#define SRC_BULLET_HPP_

#include "entity.hpp"
#include "circle.hpp"

/**
 * This class takes care of the bullet entity. This class just creates the bullets and moves them upwards
 */
class Bullet : public Entity, public Circle
{
	static constexpr float defRadius{5.f};
	static constexpr float defVelocity{10.f};
	sf::Vector2f velocity{0.f,-defVelocity};
public:
  bool isStruck{false};
  Bullet(float mX,float mY,bool updateStatus);
  ~Bullet();
  virtual void update() override;
  // When the bullet is outside the window then its dead entity
  virtual bool checkEntityDied() override;
  
  virtual void draw(sf::RenderWindow& mTarget) override;
};
#endif /* SRC_BULLET_HPP_ */
