#ifndef SRC_GAME_HPP_
#define SRC_GAME_HPP_


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <tuple>
#include <cmath>
#include <typeinfo>
#include <chrono>
#include "ball.hpp"
#include "brick.hpp"
#include "paddle.hpp"
#include "bullet.hpp"
#include "entity.hpp"
#include "lives.hpp"
#include "manager.hpp"
#include "clock.hpp"
#include "utility.hpp"
#include "MenuClass.hpp"
#include "macros.hpp"
#include "brickConfig.hxx"
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <vector>

/**
 * Game class: It contains the main game engine loop thread which runs the game.
 * It has a manager as a member variable which takes care of generating the entities and destroying them.
 * This decision is made in this class and actions are taken in the utility functions.
 */
class Game
{
  enum class GameState		// Enum for the state of the game. This is used to take several decision in the game engine loop
    {
      inprocess,paused,victory,lost,newlife,transit,gameover
    };
  
  sf::Font liberationSans;	// loading the style of font using sf::Font which will be set in sf::Text
  
  Manager manager; /*manager class which will take care of creating and destroying the entities*/
  
  GameState state; /*enum variable for setting the state of the game*/
  
  bool ifGamePaused; /*bool to check if game is paused or not*/

  int gameMode = -1;

  bool readyForupdate;
  bool updateDone;
  volatile bool startAI;
  bool timeUp;

  int currentStage; /*current stage variable which will be incremented as the stage progresses*/
  
  sf::Event windowEvent;
  
  std::unique_ptr<BoostTimer> clockPtr;
  
  /*threads responsible for several tasks related to entities*/
  std::thread spawedThread;
  std::thread mainEngineThread;
  std::thread updateEntityThread;
  std::thread AIModeThread;
  std::thread timerThread;
  std::condition_variable updateCV,AIcv;
  std::mutex mtx,AImtx;

  /**
     static and constexpr are completely independent of each other. static defines the object'S lifetime during execution. constexpr defines
     the object should be available during compilation. Compilation and execution are disjoint and discontiguous, both time and space.
     So once program is compiled, constexpr is no longer relevant

     Every variable declared constexpr is implicitly const but const and static are almost orthogonal
   **/

  sf::RenderWindow window; 		              //Game window with width,height and name string

  int gamescore; 				      // Game score

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
  
  void createEntities()
  {
    std::cout<<"creating entities"<<std::endl;
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


  void changeState(const GameState& s)
  {
    std::this_thread::sleep_for (std::chrono::milliseconds(100));         // This Delay is required because when spacebar is pressed, the bullet can not be shot immediately
    state = s;
  }

  void showStageNumberScreen()
  {
    window.clear(sf::Color::Black);
    manager.addFonts(std::make_tuple(FontType::STAGEFONT,WNDWIDTH/2.f - 70.f,WNDHEIGHT/2.f));
    manager.setFontString<FontType::STAGEFONT>("Stage: " + std::to_string(currentStage));
    window.draw(manager.getSingleFont<FontType::STAGEFONT>());
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }


  void updateEntities()
  {
    std::cout<<"update entities thread started"<<std::endl;
    while(1)
      {
	std::unique_lock<std::mutex> lck(mtx);
	updateCV.wait(lck,[this](){return readyForupdate;});
	readyForupdate = false;
	manager.update();
	updateDone = true;
	lck.unlock();
	updateCV.notify_one();
      }
  }

  void automateGame()
  {
    std::cout<<"AI thread started"<<std::endl;
    while(1)
      {
	std::unique_lock<std::mutex> lck(AImtx);
	AIcv.wait(lck,[this](){return startAI;});
	manager.predictedPaddlePosition();
	lck.unlock();
      }
  }

  void showTime(std::string timeString)
  {
    if(timeString == "0:0")
      {
	timeUp = true;
      }
    else
      {
	manager.setFontString<FontType::CLOCKFONT>(timeString);
      }
  }
  
  void startEngineLoop()
  {
    std::cout<<"main engine thread started"<<std::endl;
    while(true)
      {
	window.clear(sf::Color::Black);
	manager.setFontString<FontType::STAGEFONT>("Stage:"+std::to_string(static_cast<int>(currentStage)));
	
	window.draw(manager.getSingleFont<FontType::CLOCKFONT>());
	window.draw(manager.getSingleFont<FontType::STAGEFONT>());
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) exit(0);
	if(manager.checkBallDropped())
	  {
	    manager.handleBallDrop();
	    --manager.totalLives;
	    if(0 >= manager.totalLives)
	      {
		window.clear(sf::Color::Black);
		state = GameState::lost;
		manager.setFontString<FontType::GAMEMODEFONT>("You Lost!!");
		manager.draw(window);
		window.draw(manager.getSingleFont<FontType::GAMEMODEFONT>());
		window.display();
	      }else{
	      state = GameState::newlife;
	     
	      Paddle* paddleentity = manager.getSingleEntity<Paddle>();
	      manager.create<Ball>(paddleentity->x(),paddleentity->y()-2*Ball::defRadius,true,2.f,-2.f);
	    }
	  }
	
	// When time is up, you lost the game
	if(timeUp)
	  {
	    clockPtr->stopTimer();	    
	    window.clear(sf::Color::Black);
	    state = GameState::lost;
	    manager.setFontString<FontType::GAMEMODEFONT>("You Lost!!");
	    window.draw(manager.getSingleFont<FontType::GAMEMODEFONT>());
	    window.display();
	  }
	
	// If game is in progress and space bar is hit, then shoot bullets
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && state == GameState::inprocess)
	  {
	    if(currentStage >= 1){
	      	      Paddle* paddleentity = manager.getSingleEntity<Paddle>();
	      //	      if(manager.getAll<Bullet>().size() <= 3)
		manager.create<Bullet>(paddleentity->x(),paddleentity->y(),false);
	    }
	  }
	
	if(state == GameState::newlife)
	  {
	    {
	      std::lock_guard<std::mutex> lk(mtx);
	      Ball* mball = manager.getSingleEntity<Ball>();
	      mball->solveBallPaddleRelativeMotion();
	      readyForupdate = true;
	    }
	    updateCV.notify_one();
	    {
	      std::unique_lock<std::mutex> lk(mtx);
	      updateCV.wait(lk,[this](){return updateDone;});
	      updateDone=false;
	    }
	    manager.setFontString<FontType::SCOREFONT>("Score:"+std::to_string(gamescore));
	    manager.setFontString<FontType::LIVESFONT>("Balls:");
	    window.draw(manager.getSingleFont<FontType::SCOREFONT>());
	    window.draw(manager.getSingleFont<FontType::LIVESFONT>());
	    manager.refresh();
	    manager.draw(window);
	    window.display();
	  }
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && state == GameState::newlife)
	  {
	    {
	      std::lock_guard<std::mutex> lk(mtx);
	      Ball* ballentity = manager.getSingleEntity<Ball>();
	      ballentity->velocity.x = -2.f;
	      ballentity->velocity.y = 2.f;
	      ballentity->updateRequired = false;
	      readyForupdate = true;
	    }
	    updateCV.notify_one();
	    {
	      std::unique_lock<std::mutex> lk(mtx);
	      updateCV.wait(lk,[this](){return updateDone;});
	      updateDone=false;
	    }
	    // TODO : change this, have a better approach
	    spawedThread = std::thread([this](){changeState(GameState::inprocess);});
	    manager.setFontString<FontType::SCOREFONT>("Score:"+std::to_string(gamescore));
	    manager.setFontString<FontType::LIVESFONT>("Balls:");
	    window.draw(manager.getSingleFont<FontType::SCOREFONT>());
	    window.draw(manager.getSingleFont<FontType::LIVESFONT>());
	    manager.refresh();
	    manager.draw(window);
	    window.display();
	  }
	
	if(manager.getAll<Brick>().empty())
	  {
	    manager.setFontString<FontType::GAMEMODEFONT>("You Won!!");
	    window.draw(manager.getSingleFont<FontType::GAMEMODEFONT>());
	    manager.draw(window);
	    window.display();
	    state = GameState::inprocess;
	    currentStage++;
	    manager.clear();
	    //	    clockPtr->stopTimer();
	    //clockPtr.reset();
	    restart();
	  }
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && !ifGamePaused)
	  {
	    state = GameState::paused;
	    clockPtr->pauseTimer();
	    manager.setFontString<FontType::GAMEMODEFONT>("Paused");
	    window.draw(manager.getSingleFont<FontType::GAMEMODEFONT>());
	    ifGamePaused = true;
	    manager.draw(window);
	    window.display();
	  }
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) && ifGamePaused)
	  {
	    clockPtr->restartTimer();	    
	    state = GameState::inprocess;
	    ifGamePaused = false;
	  }
	
	if(state == GameState::inprocess)
	  {
	    {
	      std::lock_guard<std::mutex> lk(mtx);
	      readyForupdate = true;
	    }
	    updateCV.notify_one();
	    {
	      std::unique_lock<std::mutex> lk(mtx);
	      updateCV.wait(lk,[this](){return updateDone;});
	      updateDone=false;
	    }
	    
	    
	    manager.forEach<Ball>([this](Ball& mBall)		    	// resolve ball and brick collision for each brick
				  {
				    manager.forEach<Brick>([&mBall](Brick& mBrick)
							   {
							     solveBallBrickCollision(mBrick, mBall);
							   });
				  });
	    
	    manager.forEach<Ball>([this](Ball& mball)                // resolve ball and paddle collision
				  {
				    manager.forEach<Paddle>([&mball](Paddle& mPaddle)
							    {
							      solvePaddleBallCollision(mPaddle, mball);
							    });
				  });
	    
	    
	    if(manager.getAll<Bullet>().size()>0)
	      {
		manager.forEach<Brick>([this](Brick& mbrick)
				       {
					 manager.forEach<Bullet>([&mbrick](Bullet& mbullet)
								 {
								   solveBrickBulletCollision(mbrick,mbullet);
								 });
				       });
	      }
	    
	    manager.forEach<Brick>([this](Brick& brick)
				   {
				     if(brick.destroyed)
				       {
					 gamescore += brick.strength;
				       }
				   });
	    
	    manager.forEach<Brick>([this](Brick& mbrick)
				   {
				     if(mbrick.destroyed)
				       {
					 
				       }
				   }
				   );
	    manager.setFontString<FontType::SCOREFONT>("Score:"+std::to_string(gamescore));
	    manager.setFontString<FontType::LIVESFONT>("Balls:");
	    window.draw(manager.getSingleFont<FontType::SCOREFONT>());
	    window.draw(manager.getSingleFont<FontType::LIVESFONT>());
	    manager.refresh();
	    manager.draw(window);
	    window.display();
	    
	  }
	if(spawedThread.joinable())
	  {
	    spawedThread.join();
	  }
      }
  }
  
  
public:
  Game():manager(STRINGIZE_VALUE_OF(FONTFILEPATH)),window(sf::VideoMode(WNDWIDTH,WNDHEIGHT),"Arkanoid - 2"),
	 state(GameState::inprocess),
	 ifGamePaused(false),
	 readyForupdate(false),
	 updateDone(false),
	 startAI(true),
	 timeUp(false)
  {
    liberationSans.loadFromFile(STRINGIZE_VALUE_OF(FONTFILEPATH));                 // loading the font file from file system in sf::Font
    gamescore = 0; 								   // initial game score
    currentStage = 1; 								   // initial stage number
    window.setFramerateLimit(60); 						   // setting the frame rate for the window
    window.setKeyRepeatEnabled(false);
  }

  void restart()
  {
    if(currentStage == 1)
      {
	window.clear(sf::Color::Black);
	MenuManager<2> modesel(liberationSans,WNDWIDTH/2-100,WNDHEIGHT/2-50);
	modesel.setOptionsString("AI Mode","Manual Mode");
	modesel.showModeSelectWindow(window);
	while(window.isOpen()){
	  while(window.pollEvent(windowEvent))
	    {
	      if(windowEvent.type == sf::Event::KeyPressed)
		{
		  if(windowEvent.key.code == sf::Keyboard::Up || windowEvent.key.code == sf::Keyboard::Down)
		    {
		      modesel.changemode(window,windowEvent);
		    }
		  if(windowEvent.key.code == sf::Keyboard::Space)
		    {
		      gameMode = modesel.getSelectedMode();
		      break;
		    }
		}
	      else if(windowEvent.type == sf::Event::Closed)
		{
		  window.close();
		  exit(0);
		}
	      window.display();
	    }
	  if(gameMode >= 0)
	    {
	      break;
	    }
	}
      }
    createEntities();
    showStageNumberScreen();
    if(gameMode != 0)
      {
      	clockPtr.reset(new BoostTimer(0,50));
	clockPtr->setCallback(std::bind(&Game::showTime,this,std::placeholders::_1));
	clockPtr->startTimer();
      }
  }
  
  
  void run()
  {
    mainEngineThread  = std::thread([this](){startEngineLoop();});
    updateEntityThread = std::thread([this](){updateEntities();});
    if(gameMode == 0)
      {
	AIModeThread = std::thread([this](){automateGame();});
	AIModeThread.join();
      }
    else
      {
	//			clockComponent.setCallback(std::bind(&Game::showTime,this,std::placeholders::_1));
	//			clockComponent.start(0,10);
      }
    mainEngineThread.join();
    updateEntityThread.join();
  }
  
};
#endif /* SRC_GAME_HPP_ */
