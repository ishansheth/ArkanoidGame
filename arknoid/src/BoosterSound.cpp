#include "BoosterSound.hpp"

BoosterSound::BoosterSound(const std::string& fpath):SoundEntity(){
  filepath = fpath;
  soundBuffer->loadFromFile(filepath);
  sound->setBuffer(*soundBuffer);
}

void
BoosterSound::playSound(){
  sound->play();
}

void
BoosterSound::pauseSound(){
  sound->pause();
}

void
BoosterSound::setPlaybackLoop(bool flag){
  sound->setLoop(flag);
}

