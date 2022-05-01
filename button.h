#ifndef BUTTON_H
#define BUTTON_H
#include <component.h>
#include <gameobject.h>
#include <transform.h>
#include <transformbuilder.h>
#include <mainwindow.h>
#include <titlemanager.h>
#include <mapmanager.h>

class Button: public Component
{
public:
    Button();
    void onAttach() override;
    void onClick(QGraphicsSceneMouseEvent *ev) override;
    int index = 1;
    MainWindow *mainWD = nullptr;
    GameScene *gms = nullptr;
    Mapmanager *map = nullptr;
    TitleManager *tmg = nullptr;
    QString str = "按钮";
private:
    Transform *trans = nullptr;
    //ImageTransform *imgtrans = nullptr;
    QGraphicsTextItem *tag = nullptr;
    QGraphicsRectItem *Rect = nullptr;

    void generate(int index);
};

#endif // BUTTON_H
