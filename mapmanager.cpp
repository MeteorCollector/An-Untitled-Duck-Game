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

Mapmanager::Mapmanager(): Component() {}

void Mapmanager::onAttach()
{
    for(int i = 0;i < 15;i++)
        for(int j = 0;j < 20;j++)
            bmb[i][j] = nullptr;

    mplr = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mplr->setSource(QUrl("qrc:/pr/audios/bgm_sor.mp3"));
    audioOutput->setVolume(0.5f);
    mplr->setAudioOutput(audioOutput);
    mplr->setLoops(114514);
    mplr->play();
}

void Mapmanager::onUpdate(float deltaTime)
{
    if(getKeyUp(Qt::Key_R))
    {
        mplr->stop();
        return mainWD->loadScene(gms, 1);
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
