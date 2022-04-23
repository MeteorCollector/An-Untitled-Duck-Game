#include "usercontroller.h"

UserController::UserController() {}

void UserController::onAttach() {
    physics = this->gameObject->getComponent<Physics>();
    assert(physics != nullptr);
}

void UserController::onUpdate(float deltaTime) {
    float vx = 0, vy = 0;
    float velocity = 200;
    if(playerID == 0)
    {
        if(getKey(Qt::Key_A)) vx -= velocity;
        if(getKey(Qt::Key_D)) vx += velocity;
        if(getKey(Qt::Key_W)) vy -= velocity;
        if(getKey(Qt::Key_S)) vy += velocity;
    }
    if(playerID == 1)
    {
        if(getKey(Qt::Key_J)) vx -= velocity;
        if(getKey(Qt::Key_L)) vx += velocity;
        if(getKey(Qt::Key_I)) vy -= velocity;
        if(getKey(Qt::Key_K)) vy += velocity;
    }
    physics->setVelocity(vx, vy);
}
