#ifndef BRICKCOLLECTION_HPP_
#define BRICKCOLLECTION_HPP_

#include "brickConfig.hpp"
#include "EntityCollection.hpp"

class BrickCollection : public EntityCollection<Brick>
{
  int currentStage;

public:

  BrickCollection()
  {
    currentStage = 0;
  }

  void setStageNum(int stagenum)
  {
    currentStage = stagenum;
  }


  void removeBrickCollection()
  {
    clearCollection();
  }
  
  std::size_t getCollectionSize()
  {
    return entityContainer.size();
  }

  auto& getCollection() { return entityContainer; }
  
  EntityCollection::iterator begin() 
  {
    return EntityCollection::iterator(&entityContainer.front());
  }

  EntityCollection::iterator end()
  {
    return EntityCollection::iterator(&entityContainer.back());
  }
  
  void createCollection() override
  {
    if(currentStage == 0)
      return;

    // TODO: revisit the logic here
    for(int i = 0;i < brickCountX;i++)
    {
        int xThreshold = 0,k = 0;
        xThreshold = (k + brickStartCol)*(Brick::defWidth + brickSpacing);
	
        while((xThreshold + Brick::defWidth + brickSpacing) < WNDWIDTH)
        {
            float y{(k + brickStartCol)*(Brick::defWidth + brickSpacing)};
            float x{(i + brickStartRow)*(Brick::defHeight + brickSpacing)};
            const auto& conf = BrickConfig::m_brickConf[currentStage];

            auto offset = (k+i*brickCountY);

            if(offset >= brickCountX*brickCountY)
	      offset = offset % (brickCountX*brickCountY);

            if(*(conf+offset))
            {
                addEntity(brickOffsetY +y, x, sf::Color::White, 1, true);
            }
            k++;
            xThreshold += (Brick::defWidth + brickSpacing);
        }
	
      }       
  }
    
  Brick* operator[](unsigned int i) const
  {
    if(i<entityContainer.size())
      return entityContainer[i].get();
    else
      return nullptr;
  }


  bool isIntersecting(const std::shared_ptr<Brick>& mA,const Ball& mB)
  {
    return ((mA->right() >= mB.left())
	    && (mA->left() <= mB.right())
	    && (mA->bottom() >= mB.top())
	    && (mA->top() <= mB.bottom()));
  }


  void updateBricksState(Ball& mball)
  {
    for(auto& mbrick : entityContainer)
      {
	if(!mbrick->isFlying())
	  {
	    if(isIntersecting(mbrick,mball))
	      {
		--mbrick->hitsRequired;
		mball.beepSound->playSound();
		if(!mbrick->hitsRequired)
		  {
		    mbrick->flingBrick();
		  }
	    
		float overlapLeft{mball.right()-mbrick->left()};
		float overlapRight{mbrick->right()-mball.left()};
		float overlapTop{mball.bottom()-mbrick->top()};
		float overlapBottom{mbrick->bottom()-mball.top()};
		
		bool ballFromLeft{std::abs(overlapLeft) < std::abs(overlapRight)};
		bool ballFromRight{std::abs(overlapLeft) > std::abs(overlapRight)};
		
		bool ballFromTop{std::abs(overlapTop) < std::abs(overlapBottom)};
		bool ballFromBottom{std::abs(overlapTop) > std::abs(overlapBottom)};
		
		if((overlapLeft < overlapTop && overlapLeft < overlapBottom) || (overlapRight < overlapTop && overlapRight < overlapBottom)){
		  mball.velocity.x =  -mball.velocity.x;
		  break;
		}
		else if((overlapTop < overlapLeft && overlapTop < overlapRight) || (overlapBottom < overlapLeft && overlapBottom < overlapRight)){
		  mball.velocity.y =  -mball.velocity.y;
		  break;
		}
	      }
	    	
	  }
	
      }
  }

  void updateBricks()
  {
    for(auto& element:entityContainer)
      element->update();

  }

  void draw(sf::RenderWindow& window) override
  {
    for(auto& element:entityContainer)
      {
	window.draw((*element).shape);
      }
  }

};


#endif
