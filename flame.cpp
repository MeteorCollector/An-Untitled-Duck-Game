#include "flame.h"
#include "grid.h"
#include <usercontroller.h>

Flame::Flame(): Component() {}

void Flame::onAttach()
{
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
    imgtrans->setImage(":/item/images/ex_1.png");
    if (owner == 0) myowner = map->player1->getComponent<UserController>();
    else if (owner == 1) myowner = map->player2->getComponent<UserController>();
    else if (owner == 2) myowner = map->robot1->getComponent<UserController>();
    else if (owner == 3) myowner = map->robot2->getComponent<UserController>();
    stage = 0;
}

void Flame::onUpdate(float deltaTime)
{
    countdown -= deltaTime;
    if(stage == 0 && countdown < life * 3 / 4 && countdown > life / 2)
    {
        imgtrans->setImage(":/item/images/ex_2.png");
        if(map->tile[i][j]){
            auto grid = map->arr[i][j]->getComponent<Grid>();
            grid->broken();
            level = 0;
            if(map->tile[i][j] > 1) { map->tile[i][j] = 0; }// 在这里更新地图状态，因为不想在grid里再挂manager了（躺）
        }
        auto pl1 = map->player1->getComponent<UserController>();
        auto pl2 = map->player2->getComponent<UserController>();
        if(!map->pvpEnabled)
        {
            auto r1 = map->robot1->getComponent<UserController>();
            auto r2 = map->robot2->getComponent<UserController>();
            if(r1->i == i && r1->j == j && r1->isAlive) { r1->harm(4); myowner->score += 5000; myowner->labelUpdate(); }
            if(r2->i == i && r2->j == j && r2->isAlive) { r2->harm(4); myowner->score += 5000; myowner->labelUpdate(); }
        }
        if(pl1->i == i && pl1->j == j && pl1->isAlive) { pl1->harm(4); myowner->score += 5000; myowner->labelUpdate(); }
        if(pl2->i == i && pl2->j == j && pl2->isAlive) { pl2->harm(4); myowner->score += 5000; myowner->labelUpdate(); }
        if(level > 0)
        {
            switch(dir)
            {
            case 0:
                map->putFlame(i - 1, j, 1, level - 1, owner);
                map->putFlame(i + 1, j, 2, level - 1, owner);
                map->putFlame(i, j - 1, 3, level - 1, owner);
                map->putFlame(i, j + 1, 4, level - 1, owner);
                break;
            case 1:
                map->putFlame(i - 1, j, 1, level - 1, owner);
                break;
            case 2:
                map->putFlame(i + 1, j, 2, level - 1, owner);
                break;
            case 3:
                map->putFlame(i, j - 1, 3, level - 1, owner);
                break;
            case 4:
                map->putFlame(i, j + 1, 4, level - 1, owner);
                break;
            default:
                break;
            };
        }
        stage = 1;
    }
    else if(stage == 1 && countdown > life / 4)
    {
        imgtrans->setImage(":/item/images/ex_3.png");
        stage = 2;
    }
    else if(stage == 2 && countdown > 0)
    {
        imgtrans->setImage(":/item/images/ex4.png");
        stage = 3;
    }
    else if(countdown < 0){
      destory(this->gameObject);
    }
}
