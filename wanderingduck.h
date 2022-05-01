#ifndef WANDERINGDUCK_H
#define WANDERINGDUCK_H
#include <component.h>
#include <transform.h>
#include <gameobject.h>
#include <QPen>

class WanderingDuck: public Component
{
public:
    WanderingDuck();

    void onAttach() override;
    void onUpdate(float deltaTime) override;
private:
    Transform *trans = nullptr;
    QGraphicsEllipseItem *circ = nullptr;
    QMovie *movie = nullptr;
    float myTimer = 0;
    float loopTime = 4;
    int rate = 240;
    QPen *pen = nullptr;
};

#endif // WANDERINGDUCK_H
