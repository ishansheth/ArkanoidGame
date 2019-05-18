#include "ball.hpp"


// Green color of the ball is set fixed as a static variable
const sf::Color Ball::defColor{sf::Color::Green};

Ball::Ball(float mX,float mY,bool updateStatus,float xvector,float yvector):particles(2000),isParticles(false),clock(){
  velocity.x = xvector;
  velocity.y = yvector;
  shape.setPosition(mX,mY);
  shape.setRadius(defRadius);
  shape.setFillColor(defColor);
  shape.setOrigin(defRadius,defRadius);
  updateRequired = updateStatus;
  beepSound = std::make_shared<BallSound>(STRINGIZE_VALUE_OF(BEEPSOUNDFILE));
  boosterSound = std::make_shared<BoosterSound>(STRINGIZE_VALUE_OF(BOOSTERSOUNDFILE));
  particles.setEmitter(sf::Vector2f(WNDHEIGHT/2,WNDWIDTH/2));
  
}

Ball::~Ball(){}

sf::Vector2f
Ball::getVelocity(){
  return velocity;
}

void
Ball::applyParticles(){
  if(!isParticles){
    velocity.x *= 5;
    velocity.y *= 5;
      isParticles = true;
  }
}

void
Ball::disableParticles(){
  if(isParticles){
    velocity.x /= 5;
    velocity.y /= 5;
    isParticles = false;
    boosterSound->pauseSound();
    }
}

void
Ball::setPosition(float mX,float mY){
  shape.setPosition(mX,mY);
}

sf::Vector2f
Ball::getPosition(){
    return shape.getPosition();
}

void
Ball::update(){    
  sf::Time elapsed = clock.restart();
  particles.update(elapsed);
  particles.setEmitter(shape.getPosition());
  solveBoundCollisions();
  shape.move(velocity);
  if(isParticles){
    boosterSound->playSound();
  }
}

void
Ball::draw(sf::RenderWindow& mTarget){
  mTarget.draw(shape);
  if(isParticles)
    mTarget.draw(particles);
}

bool
Ball::checkEntityDied(){
  return(bottom() > WNDHEIGHT);
}

void
Ball::solveBallPaddleRelativeMotion(){
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0){
    velocity.x = -8.f;
    velocity.y = 0;
  }
  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < WNDWIDTH){
    velocity.x = 8.f;
  }else{
    velocity.x = 0;
    velocity.y = 0;
  }
  shape.move(velocity);
  
}

void
Ball::solveBoundCollisions() noexcept{
  
  if(left() < 0){
    beepSound->playSound();
    velocity.x = -velocity.x;
  }else if(right() > WNDWIDTH){
    beepSound->playSound();
    velocity.x = -velocity.x;
  }
  
  if(top() < 0){
    velocity.y = -velocity.y;
    beepSound->playSound();
  }else if(bottom() > WNDHEIGHT){
    velocity.y = -velocity.y;
    beepSound->playSound();
  }
}






