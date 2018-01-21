/*
 * clock.hpp
 *
 *  Created on: Oct 8, 2017
 *      Author: ishan
 */

#ifndef SRC_CLOCK_HPP_
#define SRC_CLOCK_HPP_

#include "macros.hpp"
#include "entity.hpp"
#include <string>
#include <thread>
#include <functional>
#include <mutex>

class Clock
{
	int min;
	int sec;
	sf::Font liberationSans;
//	std::mutex clockMutex;
	std::function<void(std::string)> callbackFunc;
	std::thread timerThread;

	public:
		Clock()
		{
			liberationSans.loadFromFile(STRINGIZE_VALUE_OF(FILEPATH));
		}


		~Clock()
		{
			if(timerThread.joinable())
			{
				timerThread.join();
			}
		}

		void setCallback(std::function<void(std::string)> bindingFunc)
		{
			callbackFunc = bindingFunc;
		}

		void resetClock(int min,int sec)
		{
			this->min = min;
			this->sec = sec;
		}

		void start(int a, int b)
		{
			min = a;sec = b;
			timerThread =  std::thread(&Clock::update,this);
		}

		void update()
		{
			while(1)
			{
				std::this_thread::sleep_for (std::chrono::seconds(1));
				sec--;
				if(sec == 0)
				{
					min--;
					if(min <= 0)
					{
						std::cout<<"time over"<<std::endl;
						break;
					}
					else
					{
						sec = 59;
					}
				}
				std::string timetext = std::to_string(min) + ":" + std::to_string(sec);
				if(callbackFunc)
					callbackFunc(timetext);
			}
		}
};
#endif /* SRC_CLOCK_HPP_ */
