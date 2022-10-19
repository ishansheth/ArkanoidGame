#ifndef SRC_LIVES_HPP_
#define SRC_LIVES_HPP_

#include "circle.hpp"
#include "entity.hpp"
#include <vector>

/**
 * The class is responsible to show the circular dots which represent the number of live player has in the game
 */
class lives : public RemovableEntity, public Circle
{
  sf::CircleShape shape;
public:
  static constexpr float defRadius{5.f};
  
  lives(float mX,float mY);

  ~lives();
  
  virtual bool checkEntityDied() override{}

  virtual void update() override{}

  void draw(sf::RenderWindow& mTarget) override;
  
};
#endif /* SRC_LIVES_HPP_ */
