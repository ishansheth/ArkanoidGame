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
  BoosterSound(const std::string& fpath);
  
  void playSound() override;
  
  void pauseSound() override;
  
  void setPlaybackLoop(bool flag) override;
};
#endif /* SRC_BOOSTERSOUND_HPP_ */
