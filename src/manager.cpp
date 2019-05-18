#include "manager.hpp"

Manager::Manager(std::string fontFilePath):fontsContainer(fontFilePath)
{}


  /**
   * The function to remove all the entites from the map and vector in which the destroyed variable is set to true
   */
void
Manager::refresh(){
  for(auto& pair: groupedEntities){
    auto& vector(pair.second);
    vector.erase(std::remove_if(std::begin(vector),std::end(vector),
				[](Entity* ment)
				{
				  return (ment->destroyed);
				}),std::end(vector));
  }
  
  entities.erase(std::remove_if(std::begin(entities),std::end(entities),
				[](std::shared_ptr<Entity> entSharedPtr)
				{
				  return (entSharedPtr->destroyed);
				}),std::end(entities));
}

void
Manager::clear(){
  groupedEntities.clear();
  entities.clear();
}

void
Manager::clearFonts(){
  fontsContainer.clear();
}

void
Manager::handleBallDrop(){
    Ball* ballentity = getSingleEntity<Ball>();
    ballentity->destroyed = true;
    
    auto vector(getAll<lives>());
    vector.back()->destroyed = true;
  }

void
Manager::enableBooster(){
  Ball* ballentity = getSingleEntity<Ball>();
  ballentity->applyParticles();
}

void
Manager::disableBooster(){
  Ball* ballentity = getSingleEntity<Ball>();
  ballentity->disableParticles();    
}

void
Manager::update(){    
  for(const auto& e: entities){
    if(!e->updateRequired){
      e->update();
    }
  }
}

void
Manager::draw(sf::RenderWindow& mTarget){
  for(auto& e: entities){
    e->draw(mTarget);
  }
}

