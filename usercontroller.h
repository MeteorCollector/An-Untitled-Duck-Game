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

    void setMove(bool emergency = false);
    bool predict(int i, int j);
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
    float myTimer = 0;
    int dy[5] = { 0, -1, 1, 0, 0 };
    int dx[5] = { 0, 0, 0, -1, 1 };
    int moving = 0;
    UserController *pl1 = nullptr;
    UserController *pl2 = nullptr;
    int targetx = 0, targety = 0;
    float loopTime = 4;
    float calmTime = 1;
    bool escaping = false;

    float dto(int i, int j, int id);
};

#endif // USERCONTROLLER_H
