#include "BulletSound.hpp"

BulletSound::BulletSound(const std::string& fpath):SoundEntity()
{
  filepath = fpath;
  soundBuffer->loadFromFile(filepath);
  sound->setBuffer(*soundBuffer);
  sound->setLoop(false);
}

void
BulletSound::playSound(){
  auto st = sound->getStatus();
  if(st == sf::SoundSource::Stopped)
    {
      std::cout<<"status::stopped "<<std::endl;
      sound->play();
    }
  else if(st == sf::SoundSource::Playing)
    {
      std::cout<<"status::playing "<<std::endl;
      sound->stop();
      sound->play();      
    }
}

void
BulletSound::pauseSound(){
  std::cout<<"stopping sound"<<std::endl;
  sound->stop();
}

void
BulletSound::setPlaybackLoop(bool flag){
  sound->setLoop(false);
}

