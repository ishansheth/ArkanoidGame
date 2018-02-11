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
#include <future>

class Stoppable
{
	std::promise<void> exitSignal;
	std::future<void> futureObj;

public:
	Stoppable():futureObj(exitSignal.get_future())
	{}

	Stoppable(Stoppable&& obj):exitSignal(std::move(obj.exitSignal)),futureObj(std::move(obj.futureObj))
	{
		std::cout<<"Move constructor is called"<<std::endl;
	}

	Stoppable& operator=(Stoppable&& obj)
	{
		std::cout<<"move assignment"<<std::endl;
		exitSignal = std::move(obj.exitSignal);
		futureObj = std::move(obj.futureObj);
		return *this;
	}

	virtual void run() = 0;

	void operator()()
	{
		run();
	}

	bool stopRequested()
	{
		if(futureObj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
			return false;
		return true;
	}

	void stop()
	{
		exitSignal.set_value();
	}
};

class Clock
{
	int min;
	int sec;
	sf::Font liberationSans;
	bool killTimerSignal;
//	std::mutex clockMutex;
	std::function<void(std::string)> callbackFunc;
	std::thread timerThread;

	public:
		Clock()
		{
			std::cout<<"creating clock object"<<std::endl;
			killTimerSignal =false;
			liberationSans.loadFromFile(STRINGIZE_VALUE_OF(FILEPATH));
		}


		~Clock()
		{
			if(timerThread.joinable())
			{
				std::cout<<"joining timer thread"<<std::endl;
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
			std::cout<<"starting new clock"<<std::endl;
			min = a;sec = b;
			timerThread =  std::thread(&Clock::runClock,this);
		}

		void killTimer()
		{
			std::cout<<"killing timer"<<std::endl;
			killTimerSignal = true;
		}

		void runClock()
		{
			while(!killTimerSignal)
			{
				std::this_thread::sleep_for (std::chrono::seconds(1));
				sec--;
				if(sec < 0)
				{
					min--;
					sec = 59;
					if(min < 0)
					{
						std::cout<<"time over"<<std::endl;
						break;
					}
				}
				std::string timetext = std::to_string(min) + ":" + std::to_string(sec);
				if(callbackFunc)
					callbackFunc(timetext);
			}
		}
};
#endif /* SRC_CLOCK_HPP_ */
