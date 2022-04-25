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

void loadScene(QMediaPlayer* player, QAudioOutput* audioOutput, float volume, GameScene *gameScene) {

    auto Manager = new Mapmanager();
    Manager->gms = gameScene;

    GameObject* Player[2];
    int spawnpoint[4][2];// 初始化玩家和机器人的生成地点
    const int width = 64, height = 48;
    int map[15][20] = { 0 };

    GameObject* flr[15][20];// 背景

    QSoundEffect effect;
    effect.setSource(QUrl::fromLocalFile(":/pr/audios/bgm_sor_wav.wav"));
    effect.setLoopCount(QSoundEffect::Infinite);
    effect.setVolume(40.0f);
    effect.play();// 失败的播放声音尝试，然而还是放在这里

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
            grid->id = map[i][j];
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

    //auto text = new QGraphicsSimpleTextItem(transform);
    //text->setText("Player 1");

    /* labeling */
    //注意，标签一定在图层最上方，所以确保这段代码被最后执行

    /*
    int dy = -50;
    ImageTransformBuilder()
          .setPos(QPointF(spawnpoint[0][0], spawnpoint[0][1] + dy))
          .setImage(":/ui/images/p1.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(Player[0]);
    ImageTransformBuilder()
          .setPos(QPointF(spawnpoint[1][0], spawnpoint[1][1] + dy))
          .setImage(":/ui/images/p2.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(Player[1]);


    //gameScene->attachGameObject(Player[1]);
    */

    /*
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
    */


    /*
    auto goose1 = new GameObject();
    auto proto = new Goose();
    proto->id = 1; proto->spawn_x = 300; proto->spawn_y = 100;
    transform->setPos(proto->spawn_x, proto->spawn_y);
    goose1->addComponent(transform);
    ImageTransformBuilder()
          .setPos(QPointF(proto->spawn_x, proto->spawn_y))
          .setImage(":/player/images/d_3.png")
          .setAlignment(Qt::AlignCenter)
          .addToGameObject(goose1);
    auto controller = new UserController();
    controller->playerID = proto->id;
    goose1->addComponent(new Physics());
    goose1->addComponent(controller);
    goose1->addComponent(proto);
    gameScene->attachGameObject(goose1);
    */

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
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    QMediaPlayer* player = new QMediaPlayer;// added audio player
    QAudioOutput* audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    loadScene(player, audioOutput, 50, gameScene);
}

MainWindow::~MainWindow() { delete ui; }
