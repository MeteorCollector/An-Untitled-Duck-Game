#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <component.h>
#include <QGraphicsTextItem>
#include <transform.h>

class MessageBox: public Component
{
public:
    MessageBox();
    void onAttach() override;
    void onUpdate(float deltaTime) override;
    float life = 0.5, velocity = 100;
    QString str;
private:
    Transform *trans = nullptr;
    QGraphicsTextItem *msg = nullptr;
};

#endif // MESSAGEBOX_H
