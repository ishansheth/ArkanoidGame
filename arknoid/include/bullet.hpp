#ifndef SRC_BULLET_HPP_
#define SRC_BULLET_HPP_

#include "entity.hpp"
#include "circle.hpp"
#include "SoundEntity.hpp"
#include "BulletSound.hpp"
#include "macros.hpp"

/**
 * This class takes care of the bullet entity. This class just creates the bullets and moves them upwards
 */
class Bullet : public sf::Drawable,public sf::Transformable
{
  static constexpr float defVelocity{15.f};

  static constexpr int bulHeight{7};
  static constexpr int bulWidth{12};
  
  sf::Vector2f velocity{0.f,-defVelocity};  
  sf::VertexArray m_TriVertices;

  std::shared_ptr<SoundEntity> bulletSound;

public:
  bool isStruck{false};
  Bullet();
  ~Bullet();
  void update();
  void setBulletPosition(float x,float y);
  virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;

  void stopSound(){bulletSound->pauseSound();}
  
  float x() const noexcept { return m_TriVertices[0].position.x; }
  float y() const noexcept { return m_TriVertices[0].position.y; }
  float width() const noexcept { return bulWidth; }
  float height() const noexcept { return bulHeight; }
  float left() const noexcept { return x() - width() / 2.f; }
  float right() const noexcept { return x() + width() / 2.f; }
  float top() const noexcept { return y() - height() / 2.f; }
  float bottom() const noexcept { return y() + height() / 2.f; }

  sf::Vector2f& getTip();

};
#endif /* SRC_BULLET_HPP_ */
