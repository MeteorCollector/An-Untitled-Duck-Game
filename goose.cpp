#include "goose.h"

#include <gameobject.h>
#include <transform.h>
#include <transformbuilder.h>
#include <physics.h>
#include <usercontroller.h>
#include <imagetransform.h>
#include <QGraphicsRectItem>

Goose::Goose() : Component() {}

void Goose::onAttach() {
  this->transform = this->gameObject->getComponent<Transform>();
  Q_ASSERT(this->transform != nullptr);
  //auto rect = new QGraphicsRectItem(this->transform);
  //rect->setRect(QRectF(-30, -30, 60, 60));
}

void Goose::onUpdate(float deltaTime) {
  /*
  cooldown -= deltaTime;
  if (cooldown > 0) return;
  cooldown = interval;

  QPointF velocity;
  if (this->getKey(Qt::Key_W))
    velocity = QPointF(0, -100);
  else if (this->getKey(Qt::Key_A))
    velocity = QPointF(-100, 0);
  else if (this->getKey(Qt::Key_S))
    velocity = QPointF(0, 100);
  else
    velocity = QPointF(100, 0);
  auto ammo = new GameObject();
  ammo->addComponent(new Transform(this->transform->pos()));
  ammo->addComponent(new Ammo(velocity));
  attachGameObject(ammo);
  */
}
