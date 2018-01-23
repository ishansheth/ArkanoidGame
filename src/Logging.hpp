#ifndef SRC_LOGGING_HPP_
#define SRC_LOGGING_HPP_

#include <chrono>
#include <string>

enum typelog {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class LOG {
public:
    LOG() : m_log(std::cout.rdbuf()) {}

    LOG(typelog type):m_log(std::cout.rdbuf())
    {
    	msglevel = type;
    }

    ~LOG()
    {
        if(opened) {
            m_log << std::endl;
        }
        opened = false;
    }

    template<class T>
    std::ostream& operator<<(const T &msg) {
        m_log<<("["+getLabel(msglevel)+"]")<<" "<< current_date()<<"  "<<msg;
        opened = true;
        return m_log;
    }
private:
    bool opened = false;
    typelog msglevel = DEBUG;
    inline std::string getLabel(typelog type) {
        std::string label;
        switch(type) {
            case DEBUG: label = "DEBUG"; break;
            case INFO:  label = "INFO "; break;
            case WARN:  label = "WARN "; break;
            case ERROR: label = "ERROR"; break;
        }
        return label;
    }
    std::string current_date(){
    	    time_t now = time(nullptr);
    	    struct tm tstruct;
    	    char buf[40];
    	    tstruct = *localtime(&now);
    	    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    	    return buf;
    	}
    std::ostream m_log;
};
#endif /* SRC_LOGGING_HPP_ */
