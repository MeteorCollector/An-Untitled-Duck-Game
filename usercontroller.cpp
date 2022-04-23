#include "usercontroller.h"
#include <transformbuilder.h>

UserController::UserController() {}

void UserController::onAttach() {
    physics = this->gameObject->getComponent<Physics>();
    assert(physics != nullptr);
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
}

void UserController::onUpdate(float deltaTime) {
    float vx = 0, vy = 0;
    int pace = 8;// 每pace步切换一次画面
    int x = imgtrans->pos().x(); x /= pace; x %= 8;
    int y = imgtrans->pos().y(); y /= pace; y %= 8;
    if(playerID == 0)// 这里复制了太多，比较粪；但是暂时没找到存很多char*的方法
    {
        if(getKey(Qt::Key_A)) {
            vx -= velocity;
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/l_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/l_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/l_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/l_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/l_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/l_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/l_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_D)) {
            vx += velocity;
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/r_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/r_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/r_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/r_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/r_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/r_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/r_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_W)) {
            vy -= velocity;
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/u_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/u_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/u_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/u_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/u_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/u_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/u_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_S)) {
            vy += velocity;
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/d_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/d_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/d_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/d_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/d_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/d_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/d_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            }
        }
    }
    else if(playerID == 1)
    {
        if(getKey(Qt::Key_J)) {
            vx -= velocity;
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/l_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/l_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/l_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/l_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/l_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/l_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/l_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/l_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_L)) {
            vx += velocity;
            switch(x){
            case 0:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/r_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/r_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/r_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/r_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/r_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/r_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/r_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/r_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_I)) {
            vy -= velocity;
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/u_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/u_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/u_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/u_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/u_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/u_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/u_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/u_1.png");
                break;
            }
        }
        if(getKey(Qt::Key_K)) {
            vy += velocity;
            switch(y){
            case 0:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            case 1:
                imgtrans->setImage(":/player/images/d_2.png");
                break;
            case 2:
                imgtrans->setImage(":/player/images/d_3.png");
                break;
            case 3:
                imgtrans->setImage(":/player/images/d_4.png");
                break;
            case 4:
                imgtrans->setImage(":/player/images/d_5.png");
                break;
            case 5:
                imgtrans->setImage(":/player/images/d_6.png");
                break;
            case 6:
                imgtrans->setImage(":/player/images/d_7.png");
                break;
            case 7:
                imgtrans->setImage(":/player/images/d_8.png");
                break;
            default:
                imgtrans->setImage(":/player/images/d_1.png");
                break;
            }
        }
    }
    physics->setVelocity(vx, vy);
}
