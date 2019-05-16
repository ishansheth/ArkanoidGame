#ifndef SRC_STAGEMANAGER_HPP_
#define SRC_STAGEMANAGER_HPP_

class StageManager
{
  
public:
  StageManager(){
  }

  ~StageManager(){}

  void createEntities()
  {
    LOG()<<DEBUG<<"creating entities";
    
    
    drawBricksForStage();
    
    manager.create<Ball>(WNDWIDTH/2.f,WNDHEIGHT/2.f,false,-2.f,2.f);		// create the ball entity
    if(gameMode == 0)
      manager.create<Paddle>(WNDWIDTH/2.f,550,true);				// create the paddle entity
    else
      manager.create<Paddle>(WNDWIDTH/2.f,550,false);				// create the paddle entity
    
    int offset = 0;							        // offset between the lives circles
    for(int i = 0; i < manager.totalLives; i++)
      {
	manager.create<lives>(550.f + offset,12.f,false);			// create the lives entity which is circles in the top right corner
	offset += 2*lives::defRadius + 2.f;
      }
    manager.addFonts(
		     std::make_tuple(FontType::LIVESFONT,490.f,2.f),
		     std::make_tuple(FontType::SCOREFONT,2.f,2.f),
		     std::make_tuple(FontType::STAGEFONT,WNDWIDTH/2.f +100.f,2.f),
		     std::make_tuple(FontType::CLOCKFONT,WNDWIDTH/2.f - 100.f,2.f),
		     std::make_tuple(FontType::GAMEMODEFONT,WNDWIDTH/2.f - 100.f,WNDHEIGHT/2.f)
		     );    
  }
  
  void drawBricksForStage(){
    for(int i = 0;i < brickCountX;i++)	      // put bricks
      {
	for(int j = 0;j < brickCountY;j++)
	  {
	    
	    float x{(i + brickStartCol*(0.7f))*(Brick::defWidth + brickSpacing)};
	    float y{(j + brickStartRow)*(Brick::defHeight + brickSpacing)};
	    auto conf = BrickConfig::m_brickConf[currentStage];
	    if(  *(conf+j*brickCountY + i)   )
	      {
		manager.create<Brick>(brickOffsetX +x ,y,sf::Color::White,1,currentStage,false);
	      }
	  }
      }       
  }
  
  
  void createEntities(){
  }
  
};
#endif /* SRC_STAGEMANAGER_HPP_ */
