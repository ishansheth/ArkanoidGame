#include "paddle.hpp"

const sf::Color Paddle::defColor{sf::Color::White};

Paddle::Paddle(float mX,float mY,bool updateStatus):angle(0),rotationAngle(0){
  shape.setPosition(mX,mY);
  lastPosition = {mX,mY};
  shape.setSize({defwidth,defheight});
  shape.setFillColor(defColor);
  shape.setOrigin(defwidth/2.f,defheight/2.f);
  updateRequired = updateStatus;
  isBullet = false;  
}

Paddle::~Paddle(){}

void
Paddle::update(){
  processPlayerInputs();
  shape.move(velocity);

  if(isBullet)
    {
      bulletContainer.erase(std::remove_if(
					   std::begin(bulletContainer),
					   std::end(bulletContainer),
					   [](Bullet& b)
					   {
					     return ((b.getTip().y < 0) || b.isStruck) ? true : false;
					   }
					   ),std::end(bulletContainer));
      
    }
    
  for(auto& ele : bulletContainer)
    {
      if(((ele.getTip().y < 0) || ele.isStruck))
       	ele.stopSound();
      ele.update();
    }
}

void
Paddle::movePaddlePosition(float mX,float mY){
  if(lastPosition.x != mX){
    if(lastPosition.x < mX){
      sf::Vector2f vel{1,0};
      while(abs(shape.getPosition().x-mX) != 0){
	// this delay is necessary to make movement of paddle observable
	std::this_thread::sleep_for (std::chrono::microseconds(800));
	shape.move(vel);
      }
      lastPosition = {mX,mY};
    }else{
      sf::Vector2f vel{-1,0};
      while(abs(shape.getPosition().x-mX) != 0){
	// this delay is necessary to make movement of paddle observable
	std::this_thread::sleep_for (std::chrono::microseconds(800));
	shape.move(vel);
      }
      lastPosition = {mX,mY};
    }
  }
}

void
Paddle::draw(sf::RenderWindow& window){
  window.draw(shape);

  if(isBullet)
    {
      for(auto& ele : bulletContainer)
	{
	    window.draw(ele);
	}
    }
}

void
Paddle::shootBullet()
{
  isBullet = true;
  Bullet bulletEnt;
  bulletEnt.setBulletPosition(shape.getPosition().x,shape.getPosition().y);
  if(bulletContainer.size() < MAX_BULLET)
    bulletContainer.push_back(bulletEnt);
}

void
Paddle::disableBullet()
{
  isBullet = false;
}

void
Paddle::processPlayerInputs(){
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
    velocity.x = -defVelocity;
  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < WNDWIDTH)
    velocity.x = defVelocity;
  else
    velocity.x = 0;
}

void
Paddle::rotatePaddle(){
  if(angle > 360.f) angle = 0.0;
  
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ){
    velocity = {(3.141f/180.f)*200*std::cos((3.141f/180.f)*angle),
		  (3.141f/180.f)*30*std::sin((3.141f/180.f)*angle)};
    angle = angle+1;
    std::cout<<"angle:"<<angle<<"    velocity:"<<velocity.x<<" "<<velocity.y<<std::endl;
  }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
    velocity = {(-1)*(3.141f/180.f)*circularPathRadius*std::cos((3.141f/180.f)*angle),
		(-1)*(3.141f/180.f)*circularPathRadius*std::sin((3.141f/180.f)*angle)};
    angle = angle-1;
    std::cout<<"angle:"<<angle<<"    velocity:"<<velocity.x<<" "<<velocity.y<<std::endl;
  }else{
    velocity = {0,0};
  }
  shape.setRotation(angle);
}





