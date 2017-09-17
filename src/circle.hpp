/*
 * circle.hpp
 *
 *  Created on: Sep 6, 2017
 *      Author: ishan
 */

#ifndef SRC_CIRCLE_HPP_
#define SRC_CIRCLE_HPP_

struct Circle
{
    sf::CircleShape shape;

    float x() const noexcept { return shape.getPosition().x; }
    float y() const noexcept { return shape.getPosition().y; }
    float radius() const noexcept { return shape.getRadius(); }
    float left() const noexcept { return x() - radius(); }
    float right() const noexcept { return x() + radius(); }
    float top() const noexcept { return y() - radius(); }
    float bottom() const noexcept { return y() + radius(); }

};





#endif /* SRC_CIRCLE_HPP_ */
