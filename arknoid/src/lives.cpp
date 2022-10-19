#include "lives.hpp"

lives::lives(float mX,float mY){
  shape.setPosition(mX,mY);
  shape.setFillColor(sf::Color::Red);
  shape.setRadius(defRadius);
  shape.setOrigin(defRadius,defRadius);
}

lives::~lives(){}

void
lives::draw(sf::RenderWindow& mTarget){
  mTarget.draw(shape);
}


