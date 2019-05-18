/*
 * starEntity.hpp
 *
 *  Created on: Nov 21, 2017
 *      Author: ishan
 */

#ifndef SRC_STARENTITY_HPP_
#define SRC_STARENTITY_HPP_
#include <vector>
#include <SFML/Graphics.hpp>


class Star : public sf::Drawable,public sf::Transformable
{
public:
  Star();
  
  Star(int x,int y,int width,int height);
  
  void setParameters(int x,int y,int width,int height);

private:
  virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
  
  sf::VertexArray m_SquareVertices;
  
  sf::VertexArray m_TriVertices1;
  sf::VertexArray m_TriVertices2;
  sf::VertexArray m_TriVertices3;
  sf::VertexArray m_TriVertices4;
  
  sf::Texture m_texture;

};

#endif /* SRC_STARENTITY_HPP_ */
