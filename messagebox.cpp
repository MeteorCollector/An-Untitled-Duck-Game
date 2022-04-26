#include "messagebox.h"
#include <QGraphicsTextItem>
#include <transform.h>
#include <gameobject.h>
#include <QFont>

MessageBox::MessageBox(): Component() {}

void MessageBox::onAttach()
{
    trans = this->gameObject->getComponent<Transform>();
    msg = new QGraphicsTextItem(trans);
    msg->setPos(-50, -60);
    auto font = new QFont();
    font->setPixelSize(12);
    font->setBold(true);
    msg->setFont(*font);
    msg->setDefaultTextColor(QColor(0, 0, 0));
    msg->setPlainText(str);

}

void MessageBox::onUpdate(float deltaTime)
{
    trans->moveBy(0, - velocity * deltaTime);
    life -= deltaTime;
    if(life < 0){ destory(this->gameObject); }
}
