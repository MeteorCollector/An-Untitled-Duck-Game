#include "titlemanager.h"
#include <gameobject.h>
#include <transform.h>
#include <transformbuilder.h>
#include <gamescene.h>
#include <mainwindow.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QFont>
#include <button.h>
#include <flame.h>

TitleManager::TitleManager(): Component() {}

void TitleManager::showtitle()
{
    inRect = new QGraphicsRectItem(trans);
    inRect->setRect(QRectF(-640, -82, 1280, 60));
    inRect->setBrush(QColor(0,0,0));
    label = new QGraphicsTextItem(trans);
    label->setPos(-480, -122);
    label->setDefaultTextColor(QColor(255, 215, 0));
    label->setPlainText("Untitled Duck Game");
    auto font = new QFont();
    font->setPixelSize(100);
    font->setBold(true);
    label->setFont(*font);
    hint = new QGraphicsTextItem(trans);
    hint->setPos(-224, 32);
    hint->setDefaultTextColor(QColor(255, 255, 255));
    hint->setPlainText(" ");
    auto hintfont = new QFont();
    hintfont->setPixelSize(36);
    hint->setFont(*hintfont);

}

void TitleManager::hidetitle()
{
    inRect->setBrush(QColor(0, 0, 0, 0));
    label->setDefaultTextColor(QColor(255, 215, 0, 0));
    hint->setDefaultTextColor(QColor(255, 255, 255, 0));
}

void TitleManager::showPVEmenu()
{

}

void TitleManager::onAttach()
{
    auto ptr = gms->getGameObject("mng");
    assert(ptr == this->gameObject);
    //if (ptr != this->gameObject) destory(this->gameObject);

    assert(gms != nullptr);
    trans = this->gameObject->getComponent<Transform>();
    mplr = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mplr->setSource(QUrl("qrc:/pr/audios/Song_for_Denise.mp3"));
    audioOutput->setVolume(0.5f);
    mplr->setAudioOutput(audioOutput);
    mplr->play();



    /* make title screen */

    Rect = new QGraphicsRectItem(trans);
    Rect->setRect(QRectF(-640, -366, 1280, 736));
    Rect->setBrush(QColor(0,0,0));

    showtitle();
}

void TitleManager::onUpdate(float deltaTime)
{
    loopTimer += deltaTime;
    myTimer += deltaTime;
    if(loopTimer > loopTime)
    {
        mplr->stop();
        loopTimer = 0;
        mplr->setSource(QUrl("qrc:/pr/audios/Song_for_Denise.mp3"));
        audioOutput->setVolume(0.5f);
        mplr->play();
    }
    if (myTimer * rate < 256) Rect->setBrush(QColor(256 - int(myTimer * rate) % 256, int(myTimer * rate) % 256, 0));
    else if (myTimer * rate < 512) Rect->setBrush(QColor(0, 256 - int(myTimer * rate) % 256, int(myTimer * rate) % 256));
    else if (myTimer * rate < 768) Rect->setBrush(QColor(int(myTimer * rate) % 256, 0, 256 - int(myTimer * rate) % 256));
    else myTimer = 0;
}
