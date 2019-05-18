#include "Logging.hpp"

LOG::LOG() : m_log(std::cout.rdbuf()),m_err(std::cerr.rdbuf()) {}

LOG::~LOG()
{
  if(opened) {
    m_log << std::endl;
  }
  opened = false;
}

std::string
LOG::getLabel(typelog type) {
  std::string label;
  switch(type) {
  case DEBUG: label = "DEBUG"; break;
  case INFO:  label = "INFO "; break;
            case WARN:  label = "WARN "; break;
  case ERROR: label = "ERROR"; break;
  }
  return label;
}


std::string
LOG::current_date(){
  time_t now = time(nullptr);
  struct tm tstruct;
  char buf[40];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%D %H:%M:%S", &tstruct);
  return buf;
}

