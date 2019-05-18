#ifndef SRC_UTILITY_HPP_
#define SRC_UTILITY_HPP_

#include<algorithm>
#include"paddle.hpp"
#include"ball.hpp"
#include"brick.hpp"
#include"bullet.hpp"
/**
 * Utility functions which are used primarily by the game class to handle the different kinds of collision
 * ball and brick, ball and paddle
 */
template<typename T1,typename T2>
bool isIntersecting(const T1& mA,const T2& mB);

void solvePaddleBallCollision(const Paddle& mpaddle, Ball& mball) noexcept;

void solveBrickBulletCollision(Brick& mbrick,Bullet& mbullet) noexcept;

void solveBallBrickCollision(Brick& mbrick, Ball& mball) noexcept;

#endif /* SRC_UTILITY_HPP_ */
