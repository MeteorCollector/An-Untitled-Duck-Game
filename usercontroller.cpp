#include "usercontroller.h"
#include <transformbuilder.h>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>

UserController::UserController() {}

void UserController::onAttach() {
    physics = this->gameObject->getComponent<Physics>();
    assert(physics != nullptr);
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
    trans = this->gameObject->getComponent<Transform>();
    assert(trans != nullptr);
    assert(map != nullptr);
    //auto circle = new QGraphicsEllipseItem(trans);
    //circle->setRect(QRectF(-22, 0, 44, 32));
    //this->collider = circle;

    // health bar
    Rect = new QGraphicsRectItem(trans);
    Rect->setRect(QRectF(- totalhealth * 3, -40, totalhealth * 6, 6));
    Rect->setBrush(Qt::black);
    Bar = new QGraphicsRectItem(trans);
    Bar->setRect(QRectF(- totalhealth * 3, -40, health * 6, 6));
    Bar->setBrush(Qt::red);

    // label
    label = new QGraphicsTextItem(trans);
    label->setPos(-40, -70);
    if(playerID == 0)
    {
        label->setDefaultTextColor(QColor(0, 255, 0));
        label->setPlainText("Player 1");
    }
    else if(playerID == 1)
    {
        label->setDefaultTextColor(QColor(0, 0, 255));
        label->setPlainText("Player 2");
    }
    auto font = new QFont();
    font->setPixelSize(20);
    font->setBold(true);
    label->setFont(*font);
    //text->setBrush(QBrush(Qt::green));
    //auto labeltrans = new Transform();
    //labeltrans->setPos(trans->pos()+QPointF(0, -20));
    //auto label = new GameObject();

}

void UserController::harm(int damage)
{
    health = health - damage < 0 ? 0 : health - damage;
    Bar->setRect(QRectF(- totalhealth * 3, -40, health * 6, 6));
}

void UserController::onUpdate(float deltaTime) {


    float vx = 0, vy = 0;
    int pace = 8;// 每pace步切换一次画面
    int x = trans->pos().x();
    int y = trans->pos().y();
    /*
        four-point judgement
        point1(-w, 0)   point2(w, 0)
        point3(0, b)  point4(0, h)
    */

    int b = 4, h = 24, w = 16;
    i = (y - 64 + 0 + 24) / 48;
    j = (x - 92 + 0 + 32) / 64;
    int i1 = (y - 64 + 0 + 24) / 48;//y1
    int j1 = (x - 92 - w + 32) / 64;//x1
    int i2 = (y - 64 + 0 + 24) / 48;//y2
    int j2 = (x - 92 + w + 32) / 64;//x2
    int i3 = (y - 64 + b + 24) / 48;//y3
    int j3 = (x - 92 + 0 + 32) / 64;//x3
    int i4 = (y - 64 + h + 24) / 48;//y4
    int j4 = (x - 92 + 0 + 32) / 64;//x4
    //Rect->setRect(QRectF(- w, b, 2 * w, h - b));

    //u_en = i > 0 && !(map->tile[i - 1][j] && (y < 64 + 48 * i - 22));// classic
    //d_en = i < 14 && !(map->tile[i + 1][j] && (y > 64 + 48 * i - 18));
    //l_en = j > 0 && !(map->tile[i][j - 1] && (x < 92 + 64 * j - 10));
    //r_en = j < 19 && !(map->tile[i][j + 1] && (x > 92 + 64 * j + 10));

    u_en = i > 0 && !((map->tile[i1 - 1][j1] || map->tile[i2 - 1][j2]) && (y < 64 + 48 * i - 22));
    d_en = i < 14 && !((map->tile[i1 + 1][j1] || map->tile[i2 + 1][j2]) && (y > 64 + 48 * i - 18));
    l_en = j > 0 && !((map->tile[i3][j3 - 1] || map->tile[i4][j4 - 1]) && (x < 92 + 64 * j - 10));
    r_en = j < 19 && !((map->tile[i3][j3 + 1] || map->tile[i4][j4 + 1]) && (x > 92 + 64 * j + 10));
    label->setPlainText("x = " + QString::number(j) + "  y = " + QString::number(i));
    x /= pace; x %= 8;
    y /= pace; y %= 8;

    if(playerID == 0)// 这里复制了太多，比较粪；但是暂时没找到存很多char*的方法
    {
        if(getKeyDown(Qt::Key_Space)){
            map->putBomb(i, j, 4, 3);
        }
        if(getKey(Qt::Key_A) && l_en) {
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
        if(getKey(Qt::Key_D) && r_en) {
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
        if(getKey(Qt::Key_W) && u_en) {
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
        if(getKey(Qt::Key_S) && d_en) {
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
        if(getKeyDown(Qt::Key_Return)){
            map->putBomb(i, j, 4, 3);
        }
        if(getKey(Qt::Key_J) && l_en) {
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
        if(getKey(Qt::Key_L) && r_en) {
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
        if(getKey(Qt::Key_I) && u_en) {
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
        if(getKey(Qt::Key_K) && d_en) {
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
