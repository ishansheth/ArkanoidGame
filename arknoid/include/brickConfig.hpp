#ifndef SRC_BRICKCONFIG_HPP_
#define SRC_BRICKCONFIG_HPP_

#include<map>

static constexpr int brickCountX{7};  	      // No of bricks in the X direction, no of columns, 11
static constexpr int brickCountY{5};   	      // No of bricks in the Y direction, no of rows , 4
static constexpr int brickStartCol{1}; 	      // start column of the bricks, 1
static constexpr int brickStartRow{2}; 	      // start row of the bricks, 2
static constexpr int brickSpacing{6};  	      // spacing between the bricks
static constexpr float brickOffsetY{22.f};    // brick offset in the X direction

static const int conf_stage1[brickCountY*brickCountX] = {
      1,1,1,1,1,
      1,1,1,1,1,
      1,1,1,1,1,
      1,1,1,1,1,
      1,1,1,1,1,
      1,1,1,1,1,
      1,1,1,1,1      
};

static const int conf_stage2[brickCountY*brickCountX] = {
      1,0,1,0,1,
      0,1,0,1,0,
      1,0,1,0,1,
      0,1,0,1,0,
      1,0,1,0,1,
      0,1,0,1,0,
      1,0,1,0,1
};

static const int conf_stage3[brickCountY*brickCountX] = {
      1,0,0,0,1,
      0,1,0,1,0,
      1,0,0,0,1,
      0,1,0,1,0,
      1,0,0,0,1,
      0,1,0,1,0,
      1,0,0,0,1
};

struct BrickConfig{
  
  static std::map<int,const int*> createBrickConfigMap();
  
  static std::map<int,const int*> m_brickConf;
};



#endif
