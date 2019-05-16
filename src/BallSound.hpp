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
	BallSound(const std::string& fpath):SoundEntity()
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
#endif /* SRC_BALLSOUND_HPP_ */
