#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <component.h>
#include <gameobject.h>
#include <physics.h>
#include <imagetransform.h>

class UserController: public Component
{
public:
    UserController();
    int playerID = 0;
    float velocity = 200;

    void onAttach() override;
    void onUpdate(float deltaTime) override;

private:
    Physics *physics;
    ImageTransform *imgtrans;
};

#endif // USERCONTROLLER_H
