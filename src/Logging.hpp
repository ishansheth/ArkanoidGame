#ifndef SRC_LOGGING_HPP_
#define SRC_LOGGING_HPP_

#include <chrono>
#include <string>
#include <type_traits>

enum typelog {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class LOG {
public:
    LOG() : m_log(std::cout.rdbuf()),m_err(std::cerr.rdbuf()) {}
    ~LOG()
    {
        if(opened) {
            m_log << std::endl;
        }
        opened = false;
    }

    template<class T>
    std::ostream& operator<<(const T &msg) {
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
    	    strftime(buf, sizeof(buf), "%D %H:%M:%S", &tstruct);
    	    return buf;
    	}
    std::ostream m_log;
    std::ostream m_err;
};
#endif /* SRC_LOGGING_HPP_ */
