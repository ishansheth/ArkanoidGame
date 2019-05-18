#include "lives.hpp"

lives::lives(float mX,float mY,bool updateStatus){
  shape.setPosition(mX,mY);
  shape.setFillColor(sf::Color::Red);
  shape.setRadius(defRadius);
  shape.setOrigin(defRadius,defRadius);
  updateRequired = updateStatus;
}

lives::~lives(){}

void
lives::draw(sf::RenderWindow& mTarget){
  mTarget.draw(shape);
}

void
lives::update(){}

bool
lives::checkEntityDied(){}


