#include "wanderingduck.h"
#include <QGraphicsItem>
#include <QBrush>
#include <QLabel>
#include <QMovie>
#include <QGraphicsEllipseItem>
#include <QPen>

WanderingDuck::WanderingDuck(): Component() {}

void WanderingDuck::onAttach()
{
    trans = this->gameObject->getComponent<Transform>();
    circ = new QGraphicsEllipseItem(trans);
    pen = new QPen();
    pen->setColor(QColor(255, 255, 255, 100));
    pen->setWidth(32);
    circ->setPen(*pen);
    circ->setRect(QRectF(-32, -32, 64, 64));
}

void WanderingDuck::onUpdate(float deltaTime)
{
    myTimer += deltaTime;
    trans->moveBy((rand() % 400 - 200) * deltaTime, (rand() % 400 - 200) * deltaTime);
    pen->setColor(QColor(255, 255, 255, 100));
    pen->setWidth(32);
    circ->setPen(*pen);
    circ->setRect(QRectF(-rate * myTimer, -rate * myTimer, rate * myTimer * 2, rate * myTimer * 2));
    if(myTimer > loopTime) myTimer = 0;
}
