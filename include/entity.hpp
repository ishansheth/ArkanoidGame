#ifndef SRC_ENTITY_HPP_
#define SRC_ENTITY_HPP_

/**
 * Base interface which has to be implemented by all the entities in the game
 */

#include <SFML/Graphics.hpp>

class Entity
{
public:
  bool destroyed{false};
  bool updateRequired{false};
  int  stagecount{1};
  virtual ~Entity(){}
  virtual void update()=0;
  virtual bool checkEntityDied()=0;
  virtual void draw(sf::RenderWindow & window)=0;
};
#endif /* SRC_ENTITY_HPP_ */
