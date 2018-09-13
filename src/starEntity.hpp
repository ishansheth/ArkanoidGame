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
  Star():m_SquareVertices(sf::Quads,4),
					 m_TriVertices1(sf::Triangles,3),
					 m_TriVertices2(sf::Triangles,3),
					 m_TriVertices3(sf::Triangles,3),
					 m_TriVertices4(sf::Triangles,3)
  {}
  
  Star(int x,int y,int width,int height):m_SquareVertices(sf::Quads,4),
					 m_TriVertices1(sf::Triangles,3),
					 m_TriVertices2(sf::Triangles,3),
					 m_TriVertices3(sf::Triangles,3),
					 m_TriVertices4(sf::Triangles,3)
  {

		m_SquareVertices[0].position = sf::Vector2f(x,y);
		m_SquareVertices[0].color = sf::Color::Red;
		
		m_SquareVertices[1].position = sf::Vector2f(y,y+width);
		m_SquareVertices[1].color = sf::Color::Red;
		
		m_SquareVertices[2].position = sf::Vector2f(x+width,y+width);
		m_SquareVertices[2].color = sf::Color::Red;

		m_SquareVertices[3].position = sf::Vector2f(x+width,y);
		m_SquareVertices[3].color = sf::Color::Red;

		m_TriVertices1[0].position = sf::Vector2f(x,y);
		m_TriVertices1[0].color = sf::Color::Green;

		m_TriVertices1[1].position = sf::Vector2f(y,y+width);
		m_TriVertices1[1].color = sf::Color::White;

		m_TriVertices1[2].position = sf::Vector2f(x-height,y+(width/2));
		m_TriVertices1[2].color = sf::Color::Blue;

		m_TriVertices2[0].position = sf::Vector2f(y,y+width);
		m_TriVertices2[0].color = sf::Color::Green;

		m_TriVertices2[1].position = sf::Vector2f(x+width,y+width);
		m_TriVertices2[1].color = sf::Color::White;

		m_TriVertices2[2].position = sf::Vector2f(x+(width/2),y+width+height);
		m_TriVertices2[2].color = sf::Color::Blue;

		m_TriVertices3[0].position = sf::Vector2f(x+width,y+width);
		m_TriVertices3[0].color = sf::Color::Green;

		m_TriVertices3[1].position = sf::Vector2f(x+width,y);
		m_TriVertices3[1].color = sf::Color::White;

		m_TriVertices3[2].position = sf::Vector2f(x+width+height,y+(width/2));
		m_TriVertices3[2].color = sf::Color::Blue;

		m_TriVertices4[0].position = sf::Vector2f(x,y);
		m_TriVertices4[0].color = sf::Color::Green;

		m_TriVertices4[1].position = sf::Vector2f(x+width,y);
		m_TriVertices4[1].color = sf::Color::White;

		m_TriVertices4[2].position = sf::Vector2f(x+(width/2),y-height);
		m_TriVertices4[2].color = sf::Color::Blue;
	}

  void setParameters(int x,int y,int width,int height){

    m_SquareVertices[0].position = sf::Vector2f(x,y);       // top left corner
    m_SquareVertices[0].color = sf::Color::Red;
		
    m_SquareVertices[1].position = sf::Vector2f(x,y+width); //top right
    m_SquareVertices[1].color = sf::Color::Red;
		
    m_SquareVertices[2].position = sf::Vector2f(x+width,y+width);   //lower right
    m_SquareVertices[2].color = sf::Color::Red;
    
    m_SquareVertices[3].position = sf::Vector2f(x+width,y);   // lower left
    m_SquareVertices[3].color = sf::Color::Red;

    m_TriVertices1[0].position = sf::Vector2f(x,y);
    m_TriVertices1[0].color = sf::Color::Green;

    m_TriVertices1[1].position = sf::Vector2f(x,y+width);
    m_TriVertices1[1].color = sf::Color::White;
    
    m_TriVertices1[2].position = sf::Vector2f(x-height,y+(width/2));
    m_TriVertices1[2].color = sf::Color::Blue;
    
    m_TriVertices2[0].position = sf::Vector2f(x,y+width);
    m_TriVertices2[0].color = sf::Color::Green;
		
    m_TriVertices2[1].position = sf::Vector2f(x+width,y+width);
    m_TriVertices2[1].color = sf::Color::White;

    m_TriVertices2[2].position = sf::Vector2f(x+(width/2),y+width+height);
    m_TriVertices2[2].color = sf::Color::Blue;

    m_TriVertices3[0].position = sf::Vector2f(x+width,y+width);
    m_TriVertices3[0].color = sf::Color::Green;

    m_TriVertices3[1].position = sf::Vector2f(x+width,y);
    m_TriVertices3[1].color = sf::Color::White;
		
    m_TriVertices3[2].position = sf::Vector2f(x+width+height,y+(width/2));
    m_TriVertices3[2].color = sf::Color::Blue;
    
    m_TriVertices4[0].position = sf::Vector2f(x,y);
    m_TriVertices4[0].color = sf::Color::Green;

    m_TriVertices4[1].position = sf::Vector2f(x+width,y);
    m_TriVertices4[1].color = sf::Color::White;

    m_TriVertices4[2].position = sf::Vector2f(x+(width/2),y-height);
    m_TriVertices4[2].color = sf::Color::Blue;
  }

private:
  virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const
  {
    //		states.transform *= getTransform();
    //		states.texture = &m_texture;
    target.draw(m_SquareVertices,states);
    target.draw(m_TriVertices1,states);
    target.draw(m_TriVertices2,states);
    target.draw(m_TriVertices3,states);
    target.draw(m_TriVertices4,states);
    
  }
  
  sf::VertexArray m_SquareVertices;
  
  sf::VertexArray m_TriVertices1;
  sf::VertexArray m_TriVertices2;
  sf::VertexArray m_TriVertices3;
  sf::VertexArray m_TriVertices4;
  
  sf::Texture m_texture;

};

#endif /* SRC_STARENTITY_HPP_ */
