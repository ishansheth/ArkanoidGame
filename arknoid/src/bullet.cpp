#include "bullet.hpp"
#include <iostream>

Bullet::Bullet():m_TriVertices(sf::Triangles,3),bulletSound(std::make_shared<BulletSound>(STRINGIZE_VALUE_OF(BULLETSOUNDFILE)))
{}

Bullet::~Bullet()
{}


void
Bullet::setBulletPosition(float x,float y)
{
  m_TriVertices[0].position = sf::Vector2f(x,y);
  m_TriVertices[0].color = sf::Color::Red;
  
  m_TriVertices[1].position = sf::Vector2f(x+(bulWidth/2),y-(bulHeight));
  m_TriVertices[1].color = sf::Color::Green;
  
  m_TriVertices[2].position = sf::Vector2f(x+bulWidth,y);
  m_TriVertices[2].color = sf::Color::Yellow;
}
  
void Bullet::update(){
  m_TriVertices[0].position += velocity ;
  m_TriVertices[1].position += velocity ;
  m_TriVertices[2].position += velocity ;
}



sf::Vector2f&
Bullet::getTip()
{
  return m_TriVertices[1].position;
}


void
Bullet::draw (sf::RenderTarget& target, sf::RenderStates states) const
{
  states.texture = NULL;
  target.draw(m_TriVertices);
  bulletSound->playSound();
}



