#include "mainwindow.h"

#include <gameobject.h>
#include <imagetransform.h>
#include <shooter.h>
#include <summondummy.h>
#include <transformbuilder.h>
#include <physics.h>
#include <usercontroller.h>
#include <grid.h>
#include <cstring>
#include <mapmanager.h>

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

void MainWindow::loadScene(GameScene *gameScene, int index) {

    if(index == 2)// die scene
    {
        gameScene->clearAll();

        //auto Manager = new Mapmanager();
        //Manager->gms = gameScene;
        //Manager->mainWD = this;
    }

    if(index == 1)// game
    {
        gameScene->clearAll();
        auto Manager = new Mapmanager();
    Manager->gms = gameScene;
    Manager->mainWD = this;
    auto mng = new GameObject();

    GameObject* Player[2];
    int spawnpoint[4][2];// 初始化玩家和机器人的生成地点
    const int width = 64, height = 48;
    int map[15][20] = { 0 };

    GameObject* flr[15][20];// 背景

    //QSoundEffect effect;
    //effect.setSource(QUrl::fromLocalFile(":/pr/audios/bgm_sor_wav.wav"));
    //effect.setLoopCount(QSoundEffect::Infinite);
    //effect.setVolume(40.0f);
    //effect.play();// 失败的播放声音尝试，然而还是放在这里

    spawnpoint[0][0] = 2, spawnpoint[0][1] = 13;
    spawnpoint[1][0] = 17, spawnpoint[1][1] = 13;

    /* map init */
    for(int i = 0; i < 20; i++)
        map[0][i] = 1;
    for(int i = 0; i < 20; i++)
        map[14][i] = 1;
    for(int i = 0; i < 15; i++)
        map[i][0] = 1;
    for(int i = 0; i < 15; i++)
        map[i][19] = 1;
    for(int i = 1; i <= 13; i++)
        for(int j = 1;j <= 18;j++)
        {
            int k = rand() % 10;
            if(k>=1 && k<=4)
                map[i][j] = k;
        }
    for(int t = 0; t <= 1; t++)// 确保玩家周围什么也没有
        for(int i = spawnpoint[t][1] - 1; i <= spawnpoint[t][1]; i++)
            for(int j = spawnpoint[t][0] - 1; j <= spawnpoint[t][0] + 1; j++)
                map[i][j] = 0;


    for(int i = 0;i < 15;i++)// 铺地板
        for(int j = 0;j < 20;j++)
            Manager->tile[i][j] = map[i][j];// 将地图信息传入公共管理系统，以后也在这个管理系统中更新。


    for(int i = 0;i < 15;i++)// 铺地板
        for(int j = 0;j < 20;j++)
        {
            flr[i][j] = new GameObject();
            auto gridPos = new Transform();
            gridPos->setPos(j * 64 + 92, i * 48 + 64);
            ImageTransformBuilder()
                  .setPos(QPointF(gridPos->pos().x(), gridPos->pos().y()))
                  .setImage(":/item/images/sand.png")
                  .setAlignment(Qt::AlignCenter)
                  .addToGameObject(flr[i][j]);
            gameScene->attachGameObject(flr[i][j]);
        }

    for(int i = 0;i < 15;i++)// 放石头
        for(int j = 0;j < 20;j++)
            //if(map[i][j])
            {
            Manager->arr[i][j] = new GameObject();
            auto grid = new Grid;
            auto gridPos = new Transform();
            grid->id = map[i][j], grid->i = i, grid->j = j;
            grid->map = Manager;
            gridPos->setPos(j * 64 + 92, i * 48 + 64);
            ImageTransformBuilder()
                  .setPos(QPointF(gridPos->pos().x(), gridPos->pos().y()))
                  .setAlignment(Qt::AlignCenter)
                  .addToGameObject(Manager->arr[i][j]);
            Manager->arr[i][j]->addComponent(grid);
            Manager->arr[i][j]->addComponent(gridPos);
            gameScene->attachGameObject(Manager->arr[i][j]);
            }


    /*
    auto obj = new GameObject();
    auto transform = new Transform();
    auto circle = new QGraphicsEllipseItem(transform);
    circle->setRect(-5, -5, 64, 64);
    transform->setPos(100, 100);
    obj->addComponent(transform);
    gameScene->attachGameObject(obj); //these are trials
    */

    /* initialize players */
    auto transform = new Transform;
    Player[0] = new GameObject();
    ImageTransformBuilder()
          .setPos(QPointF(64 * spawnpoint[0][0] + 92, 48 * spawnpoint[0][1] + 48))
          .setImage(":/player/images/d_3.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(Player[0]);
    transform->setPos(64 * spawnpoint[0][0] + 92, 48 * spawnpoint[0][1] + 48);
    auto controller1 = new UserController();
    controller1->playerID = 0;
    controller1->map = Manager;
    Player[0]->addComponent(transform);
    Player[0]->addComponent(new Physics());
    Player[0]->addComponent(controller1);
    gameScene->attachGameObject(Player[0]);

    Player[1] = new GameObject();
    ImageTransformBuilder()
          .setPos(QPointF(64 * spawnpoint[1][0] + 92, 48 * spawnpoint[1][1] + 48))
          .setImage(":/player/images/d_3.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(Player[1]);
    transform->setPos(64 * spawnpoint[1][0] + 92, 48 * spawnpoint[1][1] + 48);
    auto controller2 = new UserController();
    controller2->playerID = 1;
    controller2->map = Manager;
    Player[1]->addComponent(transform);
    Player[1]->addComponent(new Physics());
    Player[1]->addComponent(controller2);
    gameScene->attachGameObject(Player[1]);

    /* hooking */
    Manager->player1 = Player[0];
    Manager->player2 = Player[1];

    auto trans = new Transform();
    trans->setPos(700, 400);// centered!
    mng->addComponent(trans);
    mng->addComponent(Manager);
    gameScene->attachGameObject(mng);

    }
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
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    //QMediaPlayer* player = new QMediaPlayer;// added audio player
    //QAudioOutput* audioOutput = new QAudioOutput;
    //player->setAudioOutput(audioOutput);
    //connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));

    loadScene(gameScene, 1);
}

MainWindow::~MainWindow() { delete ui; }
