#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <gameobject.h>
#include <component.h>

class Mapmanager: public Component
{
public:
    Mapmanager();
    GameScene* gms = nullptr;

    int tile[15][20];
    GameObject* player1 = nullptr;
    GameObject* player2 = nullptr;
    GameObject* robot1 = nullptr;
    GameObject* robot2 = nullptr;

    void putBomb(int i, int j, int level, int countdown);
    void putFlame(int i, int j, int dir, int level);
};

#endif // MAPMANAGER_H
