#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <gameobject.h>

class Mapmanager
{
public:
    Mapmanager();
    int tile[15][20];
    GameObject* player1 = nullptr;
    GameObject* player2 = nullptr;
    GameObject* robot1 = nullptr;
    GameObject* robot2 = nullptr;
};

#endif // MAPMANAGER_H
