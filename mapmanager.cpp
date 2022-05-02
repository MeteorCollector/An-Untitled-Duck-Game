#include "mapmanager.h"
#include <gameobject.h>
#include <transform.h>
#include <transformbuilder.h>
#include <gamescene.h>
#include <bomb.h>
#include <flame.h>
#include <mainwindow.h>
#include <stuff.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QFont>

Mapmanager::Mapmanager(): Component() {}

void Mapmanager::onAttach()
{
    auto ptr = gms->getGameObject("mng");
    assert(ptr == this->gameObject);
    //if (ptr != this->gameObject) destory(this->gameObject);

    for(int i = 0;i < 15;i++)
        for(int j = 0;j < 20;j++)
            bmb[i][j] = nullptr;
    trans = this->gameObject->getComponent<Transform>();
    bgmid = 0;
    mplr = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mplr->setSource(QUrl("qrc:/pr/audios/bgm_sor.mp3"));
    audioOutput->setVolume(0.5f);
    mplr->setAudioOutput(audioOutput);
    mplr->play();
}

void Mapmanager::onUpdate(float deltaTime)
{
    if (!exitingEnabled) return;
    myTimer += deltaTime;
    //if(getKeyUp(Qt::Key_V)) victoryUI();// 调试功能，注意删除！
    if(getKeyUp(Qt::Key_R))
    {
        mplr->stop();
        //gms->clearAll();
        exitingEnabled = false;
        return mainWD->loadScene(gms, pvpEnabled? 2 : 1);
    }
    if(myTimer > 130)
    {
        myTimer = 0;
        if(bgmid == 0)
        {
            mplr->stop();
            mplr->setSource(QUrl("qrc:/pr/audios/bgm_sor.mp3"));
            audioOutput->setVolume(0.5f);
            mplr->play();
        }
    }
}

void Mapmanager::putBomb(int i, int j, int level = 1, int countdown = 3, int index = 0)
{
    bmb[i][j] = new GameObject();
    auto bomb = new Bomb();
    bomb->i = i, bomb->j = j, bomb->level = level, bomb->countdown = countdown, bomb->map = this, bomb->index = index;
    auto trans = new Transform();
    trans->setPos(j * 64 + 92, i * 48 + 64);
    ImageTransformBuilder()
          .setPos(QPointF(trans->pos().x(), trans->pos().y()))
          .setImage(":/item/images/bomb_0.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(bmb[i][j]);
    bmb[i][j]->addComponent(trans);
    bmb[i][j]->addComponent(bomb);

    gms->attachGameObject(bmb[i][j]);
}

void Mapmanager::putFlame(int i, int j, int dir, int level)
{
    auto obj = new GameObject();
    auto flame = new Flame();
    flame->i = i, flame->j = j, flame->dir = dir, flame->map = this, flame->level = level;
    auto trans = new Transform();
    trans->setPos(j * 64 + 92, i * 48 + 64);
    ImageTransformBuilder()
          .setPos(QPointF(trans->pos().x(), trans->pos().y()))
          .setImage(":/item/images/ex_1.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(obj);
    obj->addComponent(flame);
    obj->addComponent(trans);
    gms->attachGameObject(obj);
}

GameObject* Mapmanager::putStuff(int i, int j, int index)
{
    auto obj = new GameObject();
    auto stuff = new Stuff();
    stuff->index = index;
    auto trans = new Transform();
    trans->setPos(j * 64 + 92, i * 48 + 64);
    ImageTransformBuilder()
          .setPos(QPointF(trans->pos().x(), trans->pos().y()))
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(obj);
    obj->addComponent(stuff);
    obj->addComponent(trans);
    gms->attachGameObject(obj);
    return obj;
}

void Mapmanager::pvpEndUI(QString winner)
{
    if (!pvpEnabled) return;
    bgmid = 1;
    mplr->stop();
    mplr->setSource(QUrl("qrc:/pr/audios/fgo_battle_finish.mp3"));
    audioOutput->setVolume(0.9f);
    mplr->play();
    Rect = new QGraphicsRectItem(trans);
    Rect->setRect(QRectF(-640, -366, 1280, 736));
    Rect->setBrush(QColor(0,0,0,200));
    auto inRect = new QGraphicsRectItem(trans);
    inRect->setRect(QRectF(-640, -82, 1280, 60));
    inRect->setBrush(QColor(0,0,0));
    label = new QGraphicsTextItem(trans);
    label->setPos(-370, -122);
    label->setDefaultTextColor(QColor(255, 215, 0));
    label->setPlainText(winner + " wins!");
    auto font = new QFont();
    font->setPixelSize(100);
    font->setBold(true);
    label->setFont(*font);
    auto hint = new QGraphicsTextItem(trans);
    hint->setPos(-224, 32);
    hint->setDefaultTextColor(QColor(255, 255, 255));
    hint->setPlainText("Press 'R' to restart a game");
    auto hintfont = new QFont();
    hintfont->setPixelSize(36);
    hint->setFont(*hintfont);
}

void Mapmanager::victoryUI()
{
    bgmid = 1;
    mplr->stop();
    mplr->setSource(QUrl("qrc:/pr/audios/fgo_battle_finish.mp3"));
    audioOutput->setVolume(0.9f);
    mplr->play();
    Rect = new QGraphicsRectItem(trans);
    Rect->setRect(QRectF(-640, -366, 1280, 736));
    Rect->setBrush(QColor(0,0,0,200));
    auto inRect = new QGraphicsRectItem(trans);
    inRect->setRect(QRectF(-640, -82, 1280, 60));
    inRect->setBrush(QColor(0,0,0));
    label = new QGraphicsTextItem(trans);
    label->setPos(-370, -122);
    label->setDefaultTextColor(QColor(255, 215, 0));
    label->setPlainText("Battle Finished");
    auto font = new QFont();
    font->setPixelSize(100);
    font->setBold(true);
    label->setFont(*font);
    auto hint = new QGraphicsTextItem(trans);
    hint->setPos(-224, 32);
    hint->setDefaultTextColor(QColor(255, 255, 255));
    hint->setPlainText("Press 'R' to restart a game");
    auto hintfont = new QFont();
    hintfont->setPixelSize(36);
    hint->setFont(*hintfont);
}

void Mapmanager::deathUI()
{
    bgmid = 2;
    mplr->stop();
    mplr->setSource(QUrl("qrc:/pr/audios/Toby_Fox_Determination.mp3"));
    audioOutput->setVolume(1.2f);
    mplr->play();
    Rect = new QGraphicsRectItem(trans);
    Rect->setRect(QRectF(-640, -366, 1280, 736));
    Rect->setBrush(QColor(0,0,0,200));
    auto inRect = new QGraphicsRectItem(trans);
    inRect->setRect(QRectF(-640, -82, 1280, 60));
    inRect->setBrush(QColor(0,0,0));
    label = new QGraphicsTextItem(trans);
    label->setPos(-254, -122);
    label->setDefaultTextColor(QColor(255, 0, 0));
    label->setPlainText("YOU DIED");
    auto font = new QFont();
    font->setPixelSize(100);
    font->setBold(true);
    label->setFont(*font);
    auto hint = new QGraphicsTextItem(trans);
    hint->setPos(-224, 32);
    hint->setDefaultTextColor(QColor(255, 255, 255));
    hint->setPlainText("Press 'R' to restart a game");
    auto hintfont = new QFont();
    hintfont->setPixelSize(36);
    hint->setFont(*hintfont);
}
