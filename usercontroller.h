#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <component.h>
#include <gameobject.h>
#include <physics.h>
#include <imagetransform.h>
#include <mapmanager.h>
#include <QGraphicsTextItem>

class UserController: public Component
{
public:
    UserController();
    int playerID = 0;
    float velocity = 200;
    Mapmanager *map = nullptr;

    void onAttach() override;
    void onUpdate(float deltaTime) override;
    //GameObject *label = nullptr;

private:
    QGraphicsTextItem *label = nullptr;
    QGraphicsRectItem *Rect = nullptr;
    bool u_en = true, d_en = true, l_en = true, r_en = true;
    Physics *physics = nullptr;
    ImageTransform *imgtrans = nullptr;
    QGraphicsItem *collider = nullptr;
    Transform *trans = nullptr;
    //ImageTransform *labeltrans = nullptr;

};

#endif // USERCONTROLLER_H
