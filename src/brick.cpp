#include "brick.hpp"

Brick::Brick(float mX, float mY, sf::Color color,int hits,int currentstagecount,bool updateStatus){
  defColor = color;
  shape.setPosition(mX,mY);
  shape.setSize({defWidth,defHeight});
  shape.setFillColor(defColor);
  shape.setOutlineColor(sf::Color::White);
  shape.setOutlineThickness(2.f);
  shape.setOrigin(defWidth/2.f,defHeight/2.f);
  brickTexture.loadFromFile(STRINGIZE_VALUE_OF(BRICKTEXTUREIMAGEFILE));
  shape.setTexture(&brickTexture,false);
  hitsRequired = strength = hits;
  updateRequired = updateStatus;
  stagecount = currentstagecount;
}

Brick::~Brick(){}

void
Brick::draw(sf::RenderWindow& window){
  window.draw(shape);
}


void
Brick::update(){
  // if the game is in second stage onwards, then bricks will move
  if(flingFlag){
    shape.rotate(10);
    shape.move(velocityFlig);
  }
  if(x()<0){
    destroyed = true;
  }
}

void
Brick::displayEntity(sf::Window& window){
  window.setActive(true);
  window.display();
}

bool
Brick::checkEntityDied(){
  return (hitsRequired == 0);
}

bool
Brick::isFlying(){
  return flingFlag;
}

void
Brick::flingBrick(){
  flingFlag = true;
}

void
Brick::handleBrickMovements(){
  if(left()<0){
    shape.setPosition(left()+WNDWIDTH,y());
  }
}




