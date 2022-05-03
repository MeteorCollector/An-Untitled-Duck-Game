#include "bomb.h"
#include <transformbuilder.h>
#include <QSoundEffect>

Bomb::Bomb() : Component() {}

void Bomb::onAttach()
{
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
    trans = this->gameObject->getComponent<Transform>();
    assert(trans != nullptr);
    imgtrans->setImage(":/item/images/bomb_0.png");
    map->bombCnt[index] ++;
    map->bmbdata[i][j] = level;
    effect = new QSoundEffect();
    effect->setSource(QUrl("qrc:/pr/audios/fuse.wav"));
    effect->setVolume(100.0f);
    effect->play();
}

void Bomb::setMove(int dir)
{
    if(moving) return;
    int newi = i + dy[dir], newj = j + dx[dir];
    if(map->tile[newi][newj] <= 0)
    {
        targetx = 64 * (newj) + 92;
        targety = 48 * (newi) + 64;
        map->bmb[newi][newj] = map->bmb[i][j];
        map->bmb[i][j] = nullptr;
        i = newi, j = newj;
        moving = dir;
    }
}

void Bomb::onUpdate(float deltaTime)
{
    countdown -= deltaTime;
    int(countdown * 4) % 2 ? imgtrans->setImage(":/item/images/bomb_1.png") : imgtrans->setImage(":/item/images/bomb_0.png");
    if (countdown < 0) {
        map->putFlame(i, j, 0, level);
        map->bombCnt[index] --;
        map->bmb[i][j] = nullptr;
        map->bmbdata[i][j] -= level;
        effect->stop();
        effect->setSource(QUrl("qrc:/pr/audios/explode1.wav"));
        effect->setVolume(100.0f);
        effect->play();
        destory(this->gameObject);
    }

    if(moving > 0)
    {
        float x = trans->pos().x();
        float y = trans->pos().y();
        if(moving == 1)// 1 - up; 2 - down; 3 - left; 4 - right
        {
            if(y <= targety){ moving = 0; }
            trans->moveBy(0, - deltaTime * velocity);
            setMove(1);
        }
        else if(moving == 2)
        {
            if(y >= targety){ moving = 0; }
            trans->moveBy(0, deltaTime * velocity);
            setMove(2);
        }
        else if(moving == 3)
        {
            if(x <= targetx){ moving = 0; }
            trans->moveBy(- deltaTime * velocity, 0);
            setMove(3);
        }
        else if(moving == 4)
        {
            if(x >= targetx){ moving = 0; }
            trans->moveBy(deltaTime * velocity, 0);
            setMove(4);
        }
    }
}


