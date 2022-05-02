#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <queue>
#include <component.h>
#include <gameobject.h>
#include <physics.h>
#include <imagetransform.h>
#include <mapmanager.h>
#include <QGraphicsTextItem>
#include <QSoundEffect>

class UserController: public Component
{
public:
    UserController();
    int playerID = 0, bombGrade = 1, bombSum = 1;
    bool bombMove = false; bool isAlive = true;
    float velocity = 100;
    Mapmanager *map = nullptr;

    void onAttach() override;
    void onUpdate(float deltaTime) override;
    void robotUpdate(float deltaTime, int targetx, int targety);
    void setMove(float deltaTime, int x, int y, int pace);
    void harm(int damage);
    void die();
    //GameObject *label = nullptr;

    int i, j;

private:
    void say(QString str);

    std::queue<int> buff;
    int buffcount[4] = { 0 };
    QString bstr[3] = {" ", " ", " "};

    QSoundEffect *effect = nullptr;
    QGraphicsTextItem *label = nullptr;
    QGraphicsTextItem *tag = nullptr;
    QGraphicsRectItem *Rect = nullptr;
    QGraphicsRectItem *Bar = nullptr;
    bool u_en = true, d_en = true, l_en = true, r_en = true;
    Physics *physics = nullptr;
    ImageTransform *imgtrans = nullptr;
    QGraphicsItem *collider = nullptr;
    Transform *trans = nullptr;
    int health = 10, totalhealth = 10;
    //ImageTransform *labeltrans = nullptr;

    /* for AI */
    int dy[5] = { 0, -1, 1, 0, 0 };
    int dx[5] = { 0, 0, 0, -1, 1 };
    bool moving = false;

};

#endif // USERCONTROLLER_H
