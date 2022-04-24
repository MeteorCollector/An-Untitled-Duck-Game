#include "mapmanager.h"
#include <gameobject.h>
#include <transform.h>
#include <transformbuilder.h>
#include <gamescene.h>
#include <bomb.h>
#include <flame.h>

Mapmanager::Mapmanager(): Component() {}

void Mapmanager::putBomb(int i, int j, int level = 1, int countdown = 3)
{
    auto obj = new GameObject();
    auto bomb = new Bomb();
    bomb->i = i, bomb->j = j, bomb->level = level, bomb->countdown = countdown, bomb->map = this;
    auto trans = new Transform();
    trans->setPos(j * 64 + 92, i * 48 + 64);
    ImageTransformBuilder()
          .setPos(QPointF(trans->pos().x(), trans->pos().y()))
          .setImage(":/item/images/bomb_0.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(obj);
    obj->addComponent(bomb);
    obj->addComponent(trans);
    gms->attachGameObject(obj);
}

void Mapmanager::putFlame(int i, int j, int dir, int level)
{
    auto obj = new GameObject();
    auto flame = new Flame();
    flame->i = i, flame->j = j, flame->dir = dir, flame->map = this, flame->level = level;
    auto trans = new Transform();
    trans->setPos(j * 64 + 92, i * 48 + 64);
    ImageTransformBuilder()
          .setPos(QPointF(trans->pos().x(), trans->pos().y()))
          .setImage(":/item/images/ex_1.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(obj);
    obj->addComponent(flame);
    obj->addComponent(trans);
    gms->attachGameObject(obj);
}
