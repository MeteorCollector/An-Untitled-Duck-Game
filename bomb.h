#ifndef BOMB_H
#define BOMB_H

#include <component.h>
#include <transform.h>
#include <transformbuilder.h>
#include <mapmanager.h>

class Bomb: public Component
{
public:
    Bomb();
    int level = 0;
    int i, j;
    float countdown = 3;
    void onAttach() override;
    void onUpdate(float deltaTime) override;
    Mapmanager *map = nullptr;
private:
    Transform *trans = nullptr;
    ImageTransform *imgtrans = nullptr;
};

#endif // BOMB_H
