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
#include "Logging.hpp"
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
  int currentStage;
  /**
     static and constexpr are completely independent of each other. static defines the object'S lifetime during execution. constexpr defines
     the object should be available during compilation. Compilation and execution are disjoint and discontiguous, both time and space.
     So once program is compiled, constexpr is no longer relevant

     Every variable declared constexpr is implicitly const but const and static are almost orthogonal
   **/

  sf::RenderWindow window; 		              //Game window with width,height and name string

  int gamescore; 				      // Game score

  void createEntities();
  
  void drawBricksForStage();

  void showStageNumberScreen();

  void updateEntities();

  void automateGame();

  void showTime(std::string timeString);
  
  void startEngineLoop();
  
  
public:
  Game();
  
  void restart();
    
  void run();
  
};
#endif /* SRC_GAME_HPP_ */
