#ifndef BOMB_H
#define BOMB_H

#include <component.h>
#include <transform.h>
#include <transformbuilder.h>
#include <mapmanager.h>
#include <QSoundEffect>

class Bomb: public Component
{
public:
    Bomb();
    int level = 0;
    int i, j, index;
    float countdown = 3, velocity = 500;
    void onAttach() override;
    void onUpdate(float deltaTime) override;
    void setMove(int dir);
    int moving = 0;
    int targety = 0, targetx = 0;
    int dy[5] = { 0, -1, 1, 0, 0 };
    int dx[5] = { 0, 0, 0, -1, 1 };
    Mapmanager *map = nullptr;
private:
    QSoundEffect *effect = nullptr;
    Transform *trans = nullptr;
    ImageTransform *imgtrans = nullptr;
};

#endif // BOMB_H
