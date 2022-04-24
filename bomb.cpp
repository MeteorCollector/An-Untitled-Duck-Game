#include "bomb.h"
#include <transformbuilder.h>

Bomb::Bomb() : Component() {}

void Bomb::onAttach()
{
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
    imgtrans->setImage(":/item/images/bomb_0.png");
}

void Bomb::onUpdate(float deltaTime)
{
    countdown -= deltaTime;
    int(countdown * 4) % 2 ? imgtrans->setImage(":/item/images/bomb_1.png") : imgtrans->setImage(":/item/images/bomb_0.png");
    if (countdown < 0) {
        map->putFlame(i, j, 0, level);
        destory(this->gameObject);
    }
}


