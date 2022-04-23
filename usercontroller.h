#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <component.h>
#include <gameobject.h>
#include <physics.h>

class UserController: public Component
{
public:
    UserController();
    int playerID = 0;

    void onAttach() override;
    void onUpdate(float deltaTime) override;

private:
    Physics *physics;
};

#endif // USERCONTROLLER_H
