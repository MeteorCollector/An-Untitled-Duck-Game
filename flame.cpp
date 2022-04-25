#include "flame.h"
#include "grid.h"
#include <usercontroller.h>

Flame::Flame(): Component() {}

void Flame::onAttach()
{
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
    imgtrans->setImage(":/item/images/ex_1.png");
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
        if(pl1->i == i && pl1->j == j) { pl1->harm(4); }
        if(pl2->i == i && pl2->j == j) { pl2->harm(4); }
        if(level > 0)
        {
            switch(dir)
            {
            case 0:
                map->putFlame(i - 1, j, 1, level - 1);
                map->putFlame(i + 1, j, 2, level - 1);
                map->putFlame(i, j - 1, 3, level - 1);
                map->putFlame(i, j + 1, 4, level - 1);
                break;
            case 1:
                map->putFlame(i - 1, j, 1, level - 1);
                break;
            case 2:
                map->putFlame(i + 1, j, 2, level - 1);
                break;
            case 3:
                map->putFlame(i, j - 1, 3, level - 1);
                break;
            case 4:
                map->putFlame(i, j + 1, 4, level - 1);
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
