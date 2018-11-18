#ifndef SRC_BRICKCONFIG_HPP_
#define SRC_BRICKCONFIG_HPP_

#include<map>

static constexpr int brickCountX{7};  	      // No of bricks in the X direction, no of columns, 11
static constexpr int brickCountY{5};   	      // No of bricks in the Y direction, no of rows , 4
static constexpr int brickStartCol{1}; 	      // start column of the bricks, 1
static constexpr int brickStartRow{2}; 	      // start row of the bricks, 2
static constexpr int brickSpacing{6};  	      // spacing between the bricks
static constexpr float brickOffsetX{22.f};    // brick offset in the X direction

int conf_stage1[brickCountY][brickCountX] = {
      {1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1},      
};

int conf_stage2[brickCountY][brickCountX] = {
      {1,0,1,0,1,0,1},
      {0,1,0,1,0,1,0},
      {1,0,1,0,1,0,1},
      {0,1,0,1,0,1,0},
      {1,0,1,0,1,0,1},      
};

int conf_stage3[brickCountY][brickCountX] = {
      {1,0,0,1,0,0,1},
      {0,1,0,1,0,1,0},
      {0,1,1,0,1,1,0},
      {0,1,0,1,0,1,0},
      {1,0,1,0,1,0,1},      
};

struct BrickConfig{
  
  static std::map<int,int (*)[brickCountX]> createBrickConfigMap(){
    
    std::map<int,int (*)[brickCountX]> mm;
    
    mm.insert(std::pair<int,int (*)[brickCountX]>(1,conf_stage1));
    mm.insert(std::pair<int,int (*)[brickCountX]>(2,conf_stage2));
    mm.insert(std::pair<int,int (*)[brickCountX]>(3,conf_stage3));

    return mm;
  }

  static const std::map<int,int (*)[brickCountX]> m_brickConf;
};

const std::map<int,int (*)[brickCountX]> BrickConfig::m_brickConf = BrickConfig::createBrickConfigMap();


#endif
