#ifndef GOOSE_H
#define GOOSE_H

#include <cstring>
#include "component.h"
#include "health.h"
#include "hitable.h"

class Transform;

class Goose : public Component {
 public:
  Goose();
  int spawn_x = 100, spawn_y = 100;
  int id = 0;
  std::string defaultPic = ":/player/images/d_3.png";//默认图片
  bool isAlive = true;
  bool boosted = false;
  bool upgrade = false;

  void onAttach() override;
  void onUpdate(float deltaTime) override;

 protected:
  Transform *transform = nullptr;

  const float interval = .5;
  float cooldown = interval;
};

#endif // GOOSE_H
