/*
 * utility.hpp
 *
 *  Created on: Sep 16, 2017
 *      Author: ishan
 */

#ifndef SRC_UTILITY_HPP_
#define SRC_UTILITY_HPP_

template<typename T1,typename T2>
bool isIntersecting(const T1& mA,const T2& mB)
{
	return ((mA.right() >= mB.left())
			&& (mA.left() <= mB.right())
			&& (mA.bottom() >= mB.top())
			&& (mA.top() <= mB.bottom()));
}

void solvePaddleBallCollision(const Paddle& mpaddle, Ball& mball) noexcept
{
	if(!isIntersecting(mpaddle,mball)) return;
	mball.velocity.y = -Ball::defVelocity;
	mball.velocity.x = (mball.x() < mpaddle.x()) ? (-Ball::defVelocity) : (Ball::defVelocity);
}

void solveBrickBulletCollision(Brick& mbrick,Bullet& mbullet) noexcept
{
	if(!isIntersecting(mbrick,mbullet))return;
	--mbrick.hitsRequired;
	if(!mbrick.hitsRequired) mbrick.destroyed = true;
//	mbullet.isStruck = true;
	mbullet.destroyed = true;
}

void solveBallBrickCollision(Brick& mbrick, Ball& mball) noexcept
{
	if(!isIntersecting(mbrick,mball)) return;
	--mbrick.hitsRequired;
	if(!mbrick.hitsRequired) mbrick.destroyed = true;

	float overlapLeft{mball.right()-mbrick.left()};
	float overlapRight{mbrick.right()-mball.left()};
	float overlapTop{mball.bottom()-mbrick.top()};
	float overlapBottom{mbrick.bottom()-mball.top()};

	bool ballFromLeft{std::abs(overlapLeft) < std::abs(overlapRight)};
	bool ballFromTop{std::abs(overlapTop) < std::abs(overlapBottom)};

	float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
	float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

	if(std::abs(minOverlapX) < std::abs(minOverlapY))
	{
		mball.velocity.x = ballFromLeft ? -Ball::defVelocity : Ball::defVelocity;
	}
	else
	{
		mball.velocity.y = ballFromTop ? -Ball::defVelocity : Ball::defVelocity;
	}
}




#endif /* SRC_UTILITY_HPP_ */
