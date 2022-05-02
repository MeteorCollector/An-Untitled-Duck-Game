#include "button.h"
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QMessageBox>
#include <QApplication>

Button::Button(): Component() {}

void Button::onAttach(){
    assert(mainWD != nullptr);
    assert(gms != nullptr);
    trans = this->gameObject->getComponent<Transform>();
    assert(trans != nullptr);
    //imgtrans = this->gameObject->getComponent<ImageTransform>();
    generate(index);
    trans->setFiltersChildEvents(true);
}

void Button::generate(int index)
{
    Rect = new QGraphicsRectItem(trans);
    auto pen = new QPen();
    pen->setColor(QColor(255, 255, 255));
    pen->setWidth(12);
    Rect->setPen(*pen);
    Rect->setRect(QRectF(-96, -36, 192, 64));
    tag = new QGraphicsTextItem(trans);
    tag->setPos(-64, -24);
    tag->setDefaultTextColor(QColor(255, 255, 255));
    tag->setPlainText(str);
    auto tagfont = new QFont();
    tagfont->setPixelSize(24);
    tagfont->setBold(true);
    tag->setFont(*tagfont);
}

void Button::onClick(QGraphicsSceneMouseEvent *ev)
{
    if(!enabled) return;
    enabled = false;

    if(index == -1)
    {
        if (!(QMessageBox::information(NULL, tr("退出确认"), tr("真的要退出游戏吗 (T ^ T  )"), tr("果断退出"), tr("再玩一会儿"))))
        {
              QApplication* app;
              app->exit(0);
        }
    }
    else
    {
        if(tmg != nullptr) tmg->mplr->stop();
        if(map != nullptr) map->mplr->stop();
        //gms->clearAll();
        //qDebug("new scene loaded by button", 1);
        if(index == 2) return mainWD->loadScene(gms, 2);
        if(index == 1) return mainWD->loadScene(gms, 1);
        if(index == 0) return mainWD->loadScene(gms, 0);
    }

}
