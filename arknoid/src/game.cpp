#include "game.hpp"

Game::Game(sf::RenderWindow& w):manager(STRINGIZE_VALUE_OF(FONTFILEPATH)),window(sf::VideoMode(WNDWIDTH,WNDHEIGHT),"Arkanoid - 2"),window(w)
	 state(GameState::inprocess),
	 ifGamePaused(false),
	 readyForupdate(false),
	 updateDone(false),
	 timeUp(false)
{
  liberationSans.loadFromFile(STRINGIZE_VALUE_OF(FONTFILEPATH));                 // loading the font file from file system in sf::Font
  gamescore = 0; 								   // initial game score
  currentStage = 1; 								   // initial stage number
  window.setFramerateLimit(60); 						   // setting the frame rate for the window
  window.setKeyRepeatEnabled(false);
}

void
Game::restart(){
  manager.clear();
  showStageNumberScreen();
  createEntities();
  clockPtr.reset(new BoostTimer(0,50));
  clockPtr->setCallback(std::bind(&Game::showTime,this,std::placeholders::_1));
  clockPtr->startTimer();
}

void
Game::run(){
  mainEngineThread  = std::thread([this](){startEngineLoop();});
  updateEntityThread = std::thread([this](){updateEntities();});  
  mainEngineThread.join();
  updateEntityThread.join();
}


void
Game::createEntities(){
  LOG()<<DEBUG<<"creating entities";
    
  manager.createBrickCollection(currentStage);
  
  manager.create<Ball>(WNDWIDTH/2.f,WNDHEIGHT/2.f,false,-2.f,2.f);	    // create the ball entity
  manager.create<Paddle>(WNDWIDTH/2.f,550,false);			    // create the paddle entity
  
  int offset = 0;							    // offset between the lives circles
  for(int i = 0; i < manager.totalLives; i++){
    manager.create<lives>(WNDWIDTH - 50.f + offset,12.f);         // create the lives entity which is circles in the top right corner
    offset += 2*lives::defRadius + 2.f;
  }
  
  manager.addFonts(
		   std::make_tuple(FontType::LIVESFONT,WNDWIDTH-110.f,2.f),
		   std::make_tuple(FontType::SCOREFONT,2.f,2.f),       	   
		   std::make_tuple(FontType::CLOCKFONT,WNDWIDTH/2.f - 100.f,2.f),
		   std::make_tuple(FontType::GAMEMODEFONT,WNDWIDTH/2.f - 100.f,WNDHEIGHT/2.f)
		   );

  manager.getSingleFont<FontType::STAGEFONT>().setPosition(WNDWIDTH/2.f +200.f,2.f);

}

void
Game::showStageNumberScreen(){
  window.clear(sf::Color::Black);
  manager.addFonts(std::make_tuple(FontType::STAGEFONT,WNDWIDTH/2.f - 70.f,WNDHEIGHT/2.f));
  manager.setFontString<FontType::STAGEFONT>("Stage: " + std::to_string(currentStage));
  window.draw(manager.getSingleFont<FontType::STAGEFONT>());
  window.display();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  
}


void
Game::updateEntities(){
  LOG()<<DEBUG<<"update entities thread started";
  
  while(1){
    std::unique_lock<std::mutex> lck(mtx);
    updateCV.wait(lck,[this](){return readyForupdate;});
    readyForupdate = false;
    manager.update();
    updateDone = true;
    lck.unlock();
    updateCV.notify_one();
  }
}

void
Game::showTime(std::string timeString){
  if(timeString == "0:0"){
    timeUp = true;
  }else{
    manager.setFontString<FontType::CLOCKFONT>(timeString);
  }
}

void
Game::startEngineLoop(){  
  LOG()<<DEBUG<<"main engine thread started";

  sf::Texture backgrdTexture;
  backgrdTexture.loadFromFile("/home/ishan/iron-man-2-wallpapers-hd-72346-5097907.jpeg");
  sf::Sprite background(backgrdTexture);

  
  while(window.isOpen())
    {
      // just to suppress the inactivity popup during game
      window.pollEvent(windowEvent);
      
      window.draw(background);
      window.draw(manager.getSingleFont<FontType::CLOCKFONT>());
      window.draw(manager.getSingleFont<FontType::STAGEFONT>());
      
      manager.setFontString<FontType::STAGEFONT>("Stage:"+std::to_string(static_cast<int>(currentStage)));
      
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	exit(0);
      
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
		  //TODO: now what to do after this??
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
	      //now what to do after this??
	    }
      
	  // If game is in progress and ctrl is hit, then shoot bullets
	  // TODO: This power should not be available by default  
	  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && state == GameState::inprocess)
	    {
	      Paddle* mPaddle = manager.getSingleEntity<Paddle>();
	      mPaddle->shootBullet();
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
	      
	      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
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
		  state = GameState::inprocess;
		  window.draw(manager.getSingleFont<FontType::SCOREFONT>());
		  window.draw(manager.getSingleFont<FontType::LIVESFONT>());		  
		}
		
	      manager.setFontString<FontType::SCOREFONT>("Score:"+std::to_string(manager.getGameScore()));
	      manager.setFontString<FontType::LIVESFONT>("Balls:");
	      window.draw(manager.getSingleFont<FontType::SCOREFONT>());
	      window.draw(manager.getSingleFont<FontType::LIVESFONT>());
	      manager.refresh();
	      manager.draw(window);
	      window.display();
	    }

	  /*
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
	      //	    spawedThread = std::thread([this](){changeState(GameState::inprocess);});
	      state = GameState::inprocess;
	      manager.setFontString<FontType::SCOREFONT>("Score:"+std::to_string(gamescore));
	      manager.setFontString<FontType::LIVESFONT>("Balls:");
	      window.draw(manager.getSingleFont<FontType::SCOREFONT>());
	      window.draw(manager.getSingleFont<FontType::LIVESFONT>());
	      manager.refresh();
	      manager.draw(window);
	      window.display();
	    }
	  */
      
	  if(manager.isBrickContainerEmpty())
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
	  
	  if(state == GameState::inprocess){
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
	    
	    
	    manager.performEntityOperation();
	
	    manager.setFontString<FontType::SCOREFONT>("Score:"+std::to_string(manager.getGameScore()));
	    manager.setFontString<FontType::LIVESFONT>("Balls:");
	    window.draw(manager.getSingleFont<FontType::SCOREFONT>());
	    window.draw(manager.getSingleFont<FontType::LIVESFONT>());
	    manager.refresh();
	    manager.draw(window);
	    window.display();
      
	  }
    }
    
      
    /**
       if(spawedThread.joinable())
       {
       spawedThread.join();
       }**/
}


