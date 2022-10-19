#include "utility.hpp"

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
  mball.velocity.y = -abs(mball.velocity.y);
  mball.velocity.x = (mball.x() < mpaddle.x()) ? (-abs(mball.velocity.x)) : (mball.velocity.x);
}

void solveBrickBulletCollision(Brick& mbrick,Bullet& mbullet) noexcept
{
  if(isIntersecting(mbrick,mbullet))
    {
      mbullet.isStruck = true;
      --mbrick.hitsRequired;
      if(!mbrick.hitsRequired)
	{
	  mbrick.flingBrick();
	}
    }    
}



