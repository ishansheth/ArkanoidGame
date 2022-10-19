#include "BallSound.hpp"


BallSound::BallSound(const std::string& fpath):SoundEntity(){
  filepath = fpath;
  soundBuffer->loadFromFile(filepath);
  sound->setBuffer(*soundBuffer);
}

void
BallSound::playSound(){
  sound->play();
}

void
BallSound::pauseSound(){
  sound->pause();
}


void
BallSound::setPlaybackLoop(bool flag){
  sound->setLoop(flag);
}

