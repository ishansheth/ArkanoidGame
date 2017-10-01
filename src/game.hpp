#ifndef SRC_GAME_HPP_
#define SRC_GAME_HPP_


#include <SFML/Graphics.hpp>
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
#include "utility.hpp"
#include <thread>
#include <mutex>

/**
 * Game class: It contains the main game engine loop thread which runs the game.
 * It has a manager as a member variable which takes care of generating the entities and destroying them.
 * This decision is made in this class and actions are taken in the utility functions.
 */
class Game
{
	// Enum for the state of the game. This is used to take several decision in the game engine loop
	enum class GameState
	{
		inprocess,
		paused,
		victory,
		lost,
		newlife,
		transit,
		gameover
	};

	// loading the style of font using sf::Font which will be set in sf::Text
	sf::Font liberationSans;

	sf::Text textState /*text for displaying state("You won","You lost")*/,
	textLives /*text for displaying the remaining lives on the top right corner*/,
	textScore /*text for displaying the remaining lives on the top left corner*/,
	textStage /*text for displaying the stage of the game when it starts*/;

	Manager manager; /*manager class which will take care of creating and destroying the entities*/

	GameState state{GameState::inprocess}; /*enum variable for setting the state of the game*/

	bool ifGamePaused{false}; /*bool to check if game is paused or not*/

	int currentStage; /*current stage variable which will be incremented as the stage progresses*/

	/*threads responsible for several tasks related to entities*/
	std::thread spawedThread;
	std::thread textManagerThread;
	std::thread mainEngineThread;
	std::thread updateEntityThread;

	static constexpr int brickCountX{11};  /* No of bricks in the X direction, no of columns, 11*/
	static constexpr int brickCountY{4};   /* No of bricks in the Y direction, no of rows , 4*/
	static constexpr int brickStartCol{1}; /* start column of the bricks, 1*/
	static constexpr int brickStartRow{2}; /* start row of the bricks, 2*/
	static constexpr int brickSpacing{6};  /* spacing between the bricks*/
	static constexpr float brickOffsetX{22.f}; /* brick offset in the X direction*/

	sf::RenderWindow window{{wndWidth,wndHeight},"Arkanoid - 2"}; /*Game window with width,height and name string*/

	int gamescore; /* Game score */

	/**
	 * This function takes std::tuple as a parameter and sets the property of the font
	 */
	int setFontPropertiesCall(std::tuple<sf::Text&,float, float> textProp)
	{
		std::get<0>(textProp).setFont(liberationSans);
		std::get<0>(textProp).setPosition(std::get<1>(textProp),std::get<2>(textProp));
		std::get<0>(textProp).setCharacterSize(25.f);
		std::get<0>(textProp).setColor(sf::Color::White);
		// Return value is required because initializer_list is of type "int" in setFontProperties
		return 1;
	}

	/**
	 * Variadic template function which takes any number of arguments and calls the function "setFontPropertiesCall" for every passed argument
	 */
	template<typename...Ts>
	void setFontProperties(Ts&&...args)
	{
		(void)std::initializer_list<int>{(setFontPropertiesCall(args),0)...};
	}

public:
	Game()
	{
		liberationSans.loadFromFile("/home/ishan/LiberationMono-Regular.ttf"); // loading the font file from file system in sf::Font
		gamescore = 0; // initial game score
		currentStage = 1; // initial stage number
		window.setFramerateLimit(60); // setting the frame rate for the window

		// TODO : std::make_tuple(textLives,600.f,2.f) compiles but it does not change the property of the sf::Text object.
		// This can be investigated/researched : Done, pass it as a reference std::ref
		setFontProperties(
				std::make_tuple(std::ref(textLives),600.f,2.f),
				std::make_tuple(std::ref(textScore),2.f,2.f),
				std::make_tuple(std::ref(textStage),wndHeight/2.f,wndWidth/2.f),
				std::make_tuple(std::ref(textState),wndWidth/2.f - 100.f,wndHeight/2.f)
		);
	}

	void restart()
	{
		manager.clear(); // clear all the entities from the container while restart

		// put bricks
		for(int i = 0;i < brickCountX;i++)
		{
			for(int j = 0;j < brickCountY;j++)
			{
				float x{(i + brickStartCol*(0.7f))*(Brick::defWidth + brickSpacing)};
				float y{(j + brickStartRow)*(Brick::defHeight + brickSpacing)};
				if(i%2==0)
					// create brick entity which requires an update, so last parameter is false
					manager.create<Brick>(brickOffsetX +x ,y,sf::Color::Cyan,1,currentStage,false);
				else
					// create brick entity which requires an update, so last parameter is false
					manager.create<Brick>(brickOffsetX +x ,y,sf::Color::Magenta,3,currentStage,false);
			}
		}
		// create the ball entity
		manager.create<Ball>(wndWidth/2.f,wndHeight/2.f,false,-2.f,2.f);
		// create the padle entity
		manager.create<Paddle>(wndWidth/2.f,wndHeight-50,false);

		// offset between the lives circles
		int offset = 0;
		for(int i = 0; i < manager.totalLives; i++)
		{
			// create the lives entity which is circles in the top right corner
			manager.create<lives>(700.f + offset,20.f,false);
			offset += 2*lives::defRadius + 2.f;
		}
	}

	void changeState(const GameState& s)
	{
		// This Delay is required because when spacebar is pressed, the bullet can not be shot immediately
		std::this_thread::sleep_for (std::chrono::milliseconds(200));
		state = s;
	}

	void manageText()
	{
		// setting the stage text string which will be shown in the begining for 2 seconds and then fades away.
		textStage.setString("Stage: " + std::to_string(currentStage));
		// while loop which set the stage string to null when other thread sets the string to non-null
		while(1)
		{
			std::this_thread::sleep_for (std::chrono::seconds(2));
			textStage.setString("");
		}
	}

	void run()
	{
		textManagerThread = std::thread([this](){manageText();});
		mainEngineThread  = std::thread([this](){startEngineLoop();});
		textManagerThread.join();
		mainEngineThread.join();
	}

	void startEngineLoop()
	{
		while(true)
		{
			window.clear(sf::Color::Black);
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) exit(0);

			if(manager.checkBallDropped())
			{
				manager.handleBallDrop();
				--manager.totalLives;
				if(0 >= manager.totalLives)
				{
					window.clear(sf::Color::Black);
					state = GameState::lost;
					textState.setString("You Lost!!");
			    	manager.draw(window);
					window.draw(textState);
			    	window.display();
				}else{
					state = GameState::newlife;
					Paddle* paddleentity = manager.getSingleEntity<Paddle>();
			     	manager.create<Ball>(paddleentity->x(),paddleentity->y()-2*Ball::defRadius,true,-2.f,-2.f);
				}
			}

			// If game is in progress and space bar is hit, then shoot bullets
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && state == GameState::inprocess)
			{
				Paddle* paddleentity = manager.getSingleEntity<Paddle>();
				manager.create<Bullet>(paddleentity->x(),paddleentity->y(),false);
			}

			if(state == GameState::newlife)
			{
				Ball* mball = manager.getSingleEntity<Ball>();
				mball->solveBallPaddleRelativeMotion();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && state == GameState::newlife)
			{
					spawedThread = std::thread([this](){changeState(GameState::inprocess);});
					Ball* ballentity = manager.getSingleEntity<Ball>();
					ballentity->updateRequired = false;
			}

			if(manager.getAll<Brick>().empty())
			{
				//TODO: Put a delay and then break the loop
				textState.setString("You Won!!");
				manager.draw(window);
				window.draw(textState);
		    	window.display();
		    	state = GameState::inprocess;
		    	currentStage++;
		    	restart();
				textStage.setString("Stage: " + std::to_string(currentStage));
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && !ifGamePaused)
			{
				state = GameState::paused;
				textState.setString("Paused");
				ifGamePaused = true;
		    	manager.draw(window);
				window.draw(textState);
		    	window.display();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) && ifGamePaused)
			{
				state = GameState::inprocess;
				ifGamePaused = false;
			}

		    if(state == GameState::inprocess)
		    {
		    	manager.update();

		    	// resolve ball and brick collision for each brick
                manager.forEach<Ball>([this](Ball& mBall)
                {
                	manager.forEach<Brick>([&mBall](Brick& mBrick)
                    {
                		solveBallBrickCollision(mBrick, mBall);
                    });
                });

                // resolve ball and paddle collision
                manager.forEach<Ball>([this](Ball& mball)
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

                textScore.setString("Score:"+std::to_string(gamescore));
                textLives.setString("Balls:");

                window.draw(textScore);
                window.draw(textLives);
                window.draw(textStage);
		    	manager.refresh();
		    	manager.draw(window);
		    	window.display();

		    }
		    else if(state == GameState::newlife)
		    {
		    	manager.update();
                textScore.setString("Score:"+std::to_string(gamescore));
                textLives.setString("Balls:");
                window.draw(textScore);
                window.draw(textLives);
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
};

#endif /* SRC_GAME_HPP_ */
