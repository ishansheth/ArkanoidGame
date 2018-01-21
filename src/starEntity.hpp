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

class StarEntity : public sf::Drawable,public sf::Transformable
{
private:

	sf::Texture m_texture;
	sf::VertexArray m_star;

	virtual void draw(sf::RenderTarget& target,sf::RenderStates& state) const
	{
		state.transform *= getTransform();
		state.texture = &m_texture;
		target.draw(m_star,state);
	}

public:
	StarEntity():m_star(sf::TrianglesStrip)
	{

	}

};


#endif /* SRC_STARENTITY_HPP_ */
