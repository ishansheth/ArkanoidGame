#ifndef SRC_MANAGER_HPP_
#define SRC_MANAGER_HPP_

#include "entity.hpp"
#include "ball.hpp"
#include "utility.hpp"
#include "SoundEntity.hpp"
#include "BallSound.hpp"
#include "clock.hpp"
#include "FontEntity.hpp"
#include <map>
#include <vector>
#include <memory>
#include "lives.hpp"
#include "BrickCollection.hpp"
#include "brick.hpp"
#include "entity.hpp"

/**
 * Manager class which manages all the entities in the game. Creating, destroying
 */


class Manager
{
private:
  std::vector<std::shared_ptr<Entity> > entities; 					// the vector which contains the entities object
  std::map< std::size_t,std::vector<Entity*> > groupedEntities;	// the map which contains the entities object and can be accessed by the hash value of the object as a key
  
  FontEntity fontsContainer;

  BrickCollection brickEntities;

  uint64_t gamescore;
  
public:

  int totalLives{3};		     //Total number of lives the player has in the game
  
  Manager(std::string fontFilePath);
  
  Manager(const Manager& otherManager) = delete;
  Manager& operator=(const Manager& otherManager) = delete;

  /**
   * a function templated with variadic arguments which is required to generate different kind of entities in the game with varying number of arguments
   */
  template<typename T,typename...Args>
  T& create(Args&&...args){
    static_assert(std::is_base_of<Entity,T>::value,"T must be derived from the Entity interface");
    auto uPtr(std::make_shared<T>(std::forward<Args&&>(args)...));
    auto ptr(uPtr.get());
    groupedEntities[typeid(T).hash_code()].emplace_back(ptr);
    entities.emplace_back(uPtr);
    return *ptr;    
  }

  void createBrickCollection(int stagenum);

  void performEntityOperation();
  
  // variadic template for the function, it is expanded using initializer_list 
  template<typename...Ts>
  void addFonts(Ts&&...args){
    //    std::initializer_list<int>{fontsContainer.setFontProperties(args)...};
    // this also works, it expands the variabdic template inside the initializer list for each argument
    // so {fontsContainer.setFontProperties(args1),fontsContainer.setFontProperties(args2),fontsContainer.setFontProperties(args3),...}
    auto a = {fontsContainer.setFontProperties(args)...};
  }
  
  template<FontType T>
  void setFontString(std::string str){
    fontsContainer.setFontsString<T>(str);
  }

  template<FontType T>
  sf::Text& getSingleFont(){
    return fontsContainer.getTypedFont<T>();
  }
  
  /**
   * The function to remove all the entites from the map and vector in which the destroyed variable is set to true
   */
  void refresh();
  
  // clear the containers of the entities
  void clear();
   
  // The utility function which can be used to get the certain kind of entity from the map
  template<typename T>
  std::vector<Entity*> getAll(){
    return groupedEntities[typeid(T).hash_code()];
  }

  
  // The utility function which takes the entity type and functor as a template argument and calls the function with that entity as a parameter
  template<typename T, typename TFunc>
  void forEach(const TFunc& mFunc){
    auto vector(getAll<T>());
    for(const auto& entity: vector){
      mFunc(*reinterpret_cast<T*>(entity));
    }
  }
  
  template<typename T = Ball>
  bool checkBallDropped(){
    auto vector(getAll<T>());
    if(vector.size() == 1){
      RemovableEntity* ballentity = dynamic_cast<RemovableEntity*>(vector[0]);
      return ballentity->checkEntityDied();
    }else{
      return false;
    } 
  }
  
  template<typename T = Ball, typename U = Paddle>
  void predictedPaddlePosition(){
  // Be careful with the null pointers here. When entity containers are empty, casting will give null pointers
  T* ballent;
  U* padent;
  auto ballvector(getAll<T>());
  if(ballvector.size()>0)
    ballent = dynamic_cast<T*>(ballvector[0]);
  
    auto paddlevector(getAll<U>());
    if(paddlevector.size()>0)
      padent = dynamic_cast<U*>(paddlevector[0]);
    
    if(padent != nullptr && ballent != nullptr){
      if(ballent->getVelocity().y > 0 && (ballent->getVelocity().x>0 || ballent->getVelocity().x<0)){
	auto ballX = ballent->shape.getPosition().x;
	auto ballY= ballent->shape.getPosition().y;
	float intersectionX;
	if(ballent->getVelocity().x>0){
	  float c = ballY - ballX;
	  intersectionX = 550 - c;
	  if(intersectionX < 800 && intersectionX > 0){
	    padent->movePaddlePosition(intersectionX-5,550);
	  }
	}
	
	if(ballent->getVelocity().x<0){
	  float c = ballY + ballX;
	    intersectionX = (c - 550);
	    if(intersectionX < 800 && intersectionX > 0)
	      padent->movePaddlePosition(intersectionX+5,550);
	}
      }
    }

  }
  
  template<typename T>
  T* getSingleEntity(){
    auto vector(getAll<T>());
    return static_cast<T*>(vector[0]);
  }

  bool isBrickContainerEmpty()
  {
    return brickEntities.getCollectionSize() == 0;    
  }

  int getGameScore()
  {
    for(auto& br : brickEntities.getCollection())
      {
	if(br->destroyed)
	  {
	    this->gamescore += br->strength;
	  }
      }
    return gamescore;
  }
  
  void handleBallDrop();

  void enableBooster();
  
  void disableBooster();
  
  void update();
  
  void draw(sf::RenderWindow& mTarget);

};

#endif /* SRC_MANAGER_HPP_ */
