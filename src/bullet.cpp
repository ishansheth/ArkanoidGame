#include "bullet.hpp"

Bullet::Bullet(float mX,float mY,bool updateStatus){
  shape.setPosition(mX,mY);
  shape.setRadius(defRadius);
  shape.setFillColor(sf::Color::Red);
  shape.setOrigin(defRadius,defRadius);
  updateRequired = updateStatus;
}

Bullet::~Bullet(){}

void Bullet::update(){
  shape.move(velocity);
}

bool
Bullet::checkEntityDied(){
  return ( shape.getPosition().y < 0 || isStruck);
}

void
Bullet::draw(sf::RenderWindow& mTarget){
  mTarget.draw(shape);
}


