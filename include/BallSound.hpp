/*
 * BallSound.hpp
 *
 *  Created on: Oct 4, 2017
 *      Author: ishan
 */

#ifndef SRC_BALLSOUND_HPP_
#define SRC_BALLSOUND_HPP_

#include "SoundEntity.hpp"
#include "entity.hpp"

class BallSound : public SoundEntity
{
	std::string filepath;
public:
  BallSound(const std::string& fpath);
  
  void playSound() override;

  void pauseSound() override;

  void setPlaybackLoop(bool flag) override;
};
#endif /* SRC_BALLSOUND_HPP_ */
