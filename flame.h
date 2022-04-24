#ifndef FLAME_H
#define FLAME_H

#include <component.h>
#include <transform.h>
#include <transformbuilder.h>
#include <mapmanager.h>

class Flame: public Component
{
public:
    Flame();
    int i, j, dir = 0, level = 0;// 0 - 中心四向； 1 - 上， 2 - 下， 3 - 左， 4 - 右
    float countdown = 0.5f;
    float life = 0.5f;
    void onAttach() override;
    void onUpdate(float deltaTime) override;
    Mapmanager* map = nullptr;

private:
    ImageTransform* imgtrans = nullptr;
    int stage = 0;
};

#endif // FLAME_H
