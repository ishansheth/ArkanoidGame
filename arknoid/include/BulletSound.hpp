#ifndef SRC_BOOSTERSOUND_HPP_
#define SRC_BOOSTERSOUND_HPP_

#include "SoundEntity.hpp"
#include "entity.hpp"
#include <iostream>

class BulletSound : public SoundEntity
{
  std::string filepath;
public:
  BulletSound(const std::string& fpath);
  
  void playSound() override;
  
  void pauseSound() override;
  
  void setPlaybackLoop(bool flag) override;
};
#endif /* SRC_BOOSTERSOUND_HPP_ */
