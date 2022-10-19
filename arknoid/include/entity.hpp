#ifndef SRC_ENTITY_HPP_
#define SRC_ENTITY_HPP_

#include <SFML/Graphics.hpp>

/**

Base interface which has to be implemented by all the entities in the game
Every entity needs to be drawen at some point in time in the game loop
so base class loop has a only one pure virtual function

*/

class Entity
{
public:
  virtual ~Entity(){}
  virtual void draw(sf::RenderWindow & window)=0;
};

/**
the interface for the entity which needs an update during the game loop
e.g. move the location, change the state of entity etc
brick, ball, paddle are updatable,
**/

class UpdatableEntity : public Entity
{
public:
  bool updateRequired{false};
  virtual void update()=0;
};

/**
   the entity which reacts according to the user input
 */
class UserInteractiveEntity : public UpdatableEntity
{
public:
  virtual void setUerInput(sf::Keyboard::Key) = 0;
};

class RemovableEntity : public UpdatableEntity
{
protected:
public:
  bool destroyed{false};
  virtual bool checkEntityDied()=0;
};


class ResurrectableEntity : public UpdatableEntity
{
protected:
public:
  bool destroyed{false};
  virtual bool checkEntityDied()=0;
  virtual void createEntity()=0;
};
  
#endif /* SRC_ENTITY_HPP_ */
