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

#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>

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


class BoostTimer{
  boost::asio::io_service io;
  boost::asio::io_service::work work;
  boost::asio::deadline_timer m_timer;
  int m_secCount;
  int m_minCount;
  int m_startCount;
  std::function<void(std::string)> m_callbackFunc;
  std::thread timerThread;

  bool timerRunning;
  void launchTimer(){
    std::cout<<"Timerlauched\n";
    timerThread = std::thread([this](){io.run();});
  }
  
public:
  BoostTimer(int min, int sec):m_secCount(sec),
		      m_minCount(min),
		      m_startCount(0),
		      m_timer(io,boost::posix_time::seconds(1)),
			       work(io),timerRunning(false)
  {
    launchTimer();
  }
  
  void repeat(){
    if(m_startCount>0){
      m_startCount--;
      m_secCount--;
      if(m_secCount < 0){
	m_minCount--;
	m_secCount = 59;
      }
      
      if(m_minCount>=0){
	std::string timetext = std::to_string(m_minCount) + ":" + std::to_string(m_secCount);
	m_callbackFunc(timetext);
      }
      m_timer.expires_from_now(boost::posix_time::seconds(1));
      m_timer.async_wait([this](const boost::system::error_code& ec){
	  if(!ec){
	      repeat();
	  }
	});
    }
    else{
      std::cout<<"Timerover\n";
    }
  }
  
  void setCallback(std::function<void(std::string)> bindingFunc){
    m_callbackFunc = bindingFunc;
  }
  
  void startTimer(){
    m_startCount = (m_minCount*60)+m_secCount;
    repeat();
    timerRunning = true;
  }

  void resetTimer(int min, int sec){
    stopTimer();
    m_minCount = min;
    m_secCount = sec;
    m_startCount = (m_minCount*60)+m_secCount;
    repeat();
    timerRunning = true;    
  }
  
  void restartTimer(){
    timerRunning = true;
    repeat();
  }
  
  void stopTimer(){
    m_startCount = 0;
    m_minCount = 0;
    m_secCount = 0;
    m_timer.cancel();
    timerRunning = false;
  }

  void pauseTimer(){
    m_timer.cancel();
    timerRunning = true;
  }
  
  ~BoostTimer(){
    if(timerThread.joinable())
      timerThread.join();
    io.stop();
  }
  
};

#endif /* SRC_CLOCK_HPP_ */
