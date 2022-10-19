/*
 * particlesystem.hpp
 *
 *  Created on: Apr 15, 2018
 *      Author: ishan
 */

#ifndef SRC_PARTICLESYSTEM_HPP_
#define SRC_PARTICLESYSTEM_HPP_
#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
  ParticleSystem(unsigned int count);

  ~ParticleSystem();
  
  void setEmitter(sf::Vector2f position);
  
  void update(sf::Time elapsed);

private:

  struct Particle{
    sf::Vector2f velocity;
    sf::Time lifetime;
  };
  
  void resetParticle(std::size_t index);
  
  virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const;
  
  std::vector<Particle> m_particles;
  sf::VertexArray m_vertices;
  sf::Time m_lifetime;
  sf::Vector2f m_emitter;
  
};



#endif /* SRC_PARTICLESYSTEM_HPP_ */
