#include "usercontroller.h"

UserController::UserController() {}

void UserController::onAttach() {
    physics = this->gameObject->getComponent<Physics>();
    assert(physics != nullptr);
}

void UserController::onUpdate(float deltaTime) {
    float vx = 0, vy = 0;
    float v = 233;
    if(getKey(Qt::Key_A)) vx -= v;
    if(getKey(Qt::Key_D)) vx += v;
    if(getKey(Qt::Key_W)) vy -= v;
    if(getKey(Qt::Key_S)) vy += v;
    physics->setVelocity(vx, vy);
}
