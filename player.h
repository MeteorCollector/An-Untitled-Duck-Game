#ifndef PLAYER_H
#define PLAYER_H

#include "component.h"
#include "health.h"
#include "hitable.h"

class Transform;

class Player : public Component {
 public:
  Player();

  void onAttach() override;
  void onUpdate(float deltaTime) override;

 protected:
  Transform *transform = nullptr;

  const float interval = .5;
  float cooldown = interval;
};

#endif // PLAYER_H
