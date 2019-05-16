/*
 * BallSound.hpp
 *
 *  Created on: Oct 4, 2017
 *      Author: ishan
 */

#ifndef SRC_BOOSTERSOUND_HPP_
#define SRC_BOOSTERSOUND_HPP_

#include "SoundEntity.hpp"
#include "entity.hpp"

class BoosterSound : public SoundEntity
{
	std::string filepath;
public:
  BoosterSound(const std::string& fpath):SoundEntity()
  {
    filepath = fpath;
    soundBuffer->loadFromFile(filepath);
    sound->setBuffer(*soundBuffer);
  }
  
  void playSound() override
  {
    sound->play();
  }
  
  void pauseSound() override
  {
    sound->pause();
  }
  
  void setPlaybackLoop(bool flag) override
  {
    sound->setLoop(flag);
  }
};
#endif /* SRC_BOOSTERSOUND_HPP_ */
