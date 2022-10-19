#ifndef SRC_LOGGING_HPP_
#define SRC_LOGGING_HPP_

#include <chrono>
#include <string>
#include <type_traits>
#include <iostream>

enum typelog {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class LOG {
public:
  LOG();

  ~LOG();

  template<class T>
  std::ostream& operator<<(const T &msg){
    opened = true;
    if(std::is_same<T,typelog>::value)
      {
	if(msg != DEBUG)
	  {
	    m_log<<current_date()<<"  "<<("["+getLabel(msg)+"]");
	    return m_log;
	  }else{
	  m_err<<current_date()<<"  "<<("["+getLabel(msg)+"]");
	  return m_err;
      }
      }   
  }
  
private:
  bool opened = false;

  typelog msglevel = DEBUG;

  std::string getLabel(typelog type);

  std::string current_date();

  std::ostream m_log;

  std::ostream m_err;
};
#endif /* SRC_LOGGING_HPP_ */
