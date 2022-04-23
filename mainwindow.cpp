#include "mainwindow.h"

#include <gameobject.h>
#include <imagetransform.h>
#include <shooter.h>
#include <summondummy.h>
#include <transformbuilder.h>
#include <physics.h>
#include <usercontroller.h>

#include "./ui_mainwindow.h"

#include <QGraphicsEllipseItem> //trial
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

void playmusic(QMediaPlayer* player, QAudioOutput* audioOutput, float volume)
{
    player->setSource(QUrl::fromLocalFile(":/pr/audios/bgm_sor.mp3"));
    audioOutput->setVolume(50);
    player->setLoops(-1);// loops forever
    player->play();
    return;
}

void loadScene(QMediaPlayer* player, QAudioOutput* audioOutput, float volume, GameScene *gameScene) {
    QSoundEffect effect;
    effect.setSource(QUrl::fromLocalFile(":/pr/audios/bgm_sor_wav.wav"));
    effect.setLoopCount(QSoundEffect::Infinite);
    effect.setVolume(40.0f);
    effect.play();

    auto obj = new GameObject();
    auto transform = new Transform();
    auto circle = new QGraphicsEllipseItem(transform);
    circle->setRect(-5, -5, 64, 64);
    transform->setPos(100, 100);
    obj->addComponent(transform);
    gameScene->attachGameObject(obj); //these are trials
    auto pic = new GameObject();
    ImageTransformBuilder()
          .setPos(QPointF(300, 100))
          .setImage(":/player/images/d_3.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(pic);
    transform->setPos(300, 100);
    pic->addComponent(transform);
    pic->addComponent(new Physics());
    pic->addComponent(new UserController());
    gameScene->attachGameObject(pic); //image trial

    /*
    auto shooter = new GameObject();
    ImageTransformBuilder()
        .setPos(QPointF(100, 100))
        .setImage("enemy_3_0.png")
       .setAlignment(Qt::AlignCenter)
       .addToGameObject(shooter);
    shooter->addComponent(new Shooter);
    gameScene->attachGameObject(shooter);

    auto summonDummyBtn = new GameObject();
     summonDummyBtn->addComponent(new Transform);
    summonDummyBtn->addComponent(new SummonDummy);
     gameScene->attachGameObject(summonDummyBtn);
     */
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    gameScene = new GameScene();
    gameScene->setParent(this);
    auto view = new QGraphicsView(gameScene, this);
    view->setFrameStyle(QFrame::NoFrame);
    view->resize(this->size());
    view->setSceneRect(QRect(0, 0, this->width(), this->height()));

    QMediaPlayer* player = new QMediaPlayer;// added audio player
    QAudioOutput* audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    loadScene(player, audioOutput, 50, gameScene);
}

MainWindow::~MainWindow() { delete ui; }
