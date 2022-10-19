#include "brickConfig.hpp"


std::map<int,const int*> BrickConfig::m_brickConf = BrickConfig::createBrickConfigMap();

std::map<int,const int*> BrickConfig::createBrickConfigMap(){
    
    std::map<int,const int*> mm;
    mm.insert(std::pair<int,const int*>(2,conf_stage3));
    mm.insert(std::pair<int,const int*>(1,conf_stage2));
    mm.insert(std::pair<int,const int*>(3,conf_stage3));

    return mm;
  }
