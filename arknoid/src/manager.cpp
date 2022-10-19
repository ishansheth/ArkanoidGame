#include "manager.hpp"

Brick* begin(BrickCollection& col)
{
  return col.getEntityAt(0);
}

Brick* end(BrickCollection& col)
{
  return col.getEntityAt(0+col.getCollectionSize());
}

Manager::Manager(std::string fontFilePath):fontsContainer(fontFilePath),brickEntities(),gamescore(0)
{}


/**
 * The function to remove all the entites from the map and vector in which the destroyed variable is set to true
 */
void
Manager::refresh(){
  for(auto& pair: groupedEntities)
    {
      auto& vector(pair.second);
      vector.erase(std::remove_if(std::begin(vector),std::end(vector),
				  [](Entity* ment)
				  {
				    if(dynamic_cast<RemovableEntity*>(ment) != nullptr)
				      return (dynamic_cast<RemovableEntity*>(ment)->destroyed);
				    else
				      return false;
				  }),std::end(vector));
    }
  
  entities.erase(std::remove_if(std::begin(entities),std::end(entities),
				[](std::shared_ptr<Entity> entSharedPtr)
				{
				  std::shared_ptr<RemovableEntity> remEntity = std::dynamic_pointer_cast<RemovableEntity>(entSharedPtr);
				  if(remEntity != nullptr)
				    return remEntity->destroyed;
				  else
				    return false;
				}),std::end(entities));

  
  auto& brickColl = brickEntities.getCollection();
  
  brickColl.erase(std::remove_if(std::begin(brickColl),std::end(brickColl),
				     [](std::shared_ptr<Brick> entSharedPtr)
				     {
				       return entSharedPtr->destroyed;
				     }),std::end(brickColl));
  
}

void
Manager::performEntityOperation()
{  
  forEach<Ball>(
		[this](Ball& mball)
		{
		  brickEntities.updateBricksState(mball);
		});
  
      
  forEach<Ball>([this](Ball& mball)                // resolve ball and paddle collision
		{
		  forEach<Paddle>([&mball](Paddle& mPaddle)
				  {
				    solvePaddleBallCollision(mPaddle, mball);
				  });
		});


  for(auto br : brickEntities)
    {
      for(auto& bu : getSingleEntity<Paddle>()->getBulletContainer())
	{
	  solveBrickBulletCollision(*br,bu);
	}
    }
}

void
Manager::createBrickCollection(int stagenum)
  {
    brickEntities.setStageNum(stagenum);
    brickEntities.createCollection();
  }


void
Manager::clear(){
  groupedEntities.clear();
  entities.clear();
  brickEntities.clearCollection();
  fontsContainer.clear();
}


void
Manager::handleBallDrop()
{
    Ball* ballentity = getSingleEntity<Ball>();
    ballentity->destroyed = true;    
    auto vector(getAll<lives>());
    dynamic_cast<RemovableEntity*>(vector.back())->destroyed = true;
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
  brickEntities.updateBricks();
  for(const auto& e: entities){
    std::shared_ptr<UpdatableEntity> u = std::dynamic_pointer_cast<UpdatableEntity>(e);
    if(u != nullptr)
      {
	if(!u->updateRequired)
	  {
	    u->update();
	  }
      }
  }
}

void
Manager::draw(sf::RenderWindow& mTarget)
{
  brickEntities.draw(mTarget);
  
  for(auto& e: entities){
    e->draw(mTarget);
  }
}

