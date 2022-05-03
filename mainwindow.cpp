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
#include <titlemanager.h>
#include <wanderingduck.h>
#include <button.h>

#include "./ui_mainwindow.h"

#include <QGraphicsEllipseItem> //trial
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QLabel>
#include <QMovie>


void playmusic(QMediaPlayer* player, QAudioOutput* audioOutput, float volume)
{
    player->setSource(QUrl::fromLocalFile(":/pr/audios/bgm_sor.mp3"));
    audioOutput->setVolume(50);
    player->setLoops(-1);// loops forever
    player->play();
    return;
}

void MainWindow::loadScene(GameScene *gameScene, int index) {

    qDebug("loadscene,");
    if(index == 0)// title scene
    {
        //gameScene->clear();
        gameScene->clearAll();


        auto Manager = new TitleManager();
        auto mng = new GameObject();
        Manager->gms = gameScene;
        Manager->mainWD = this;

        auto trans = new Transform();
        trans->setPos(700, 400);// centered!
        mng->addComponent(trans);
        mng->addComponent(Manager);
        gameScene->attachGameObject(mng);

        //GameObject* ptr = gameScene->getGameObject("mng");
        //if (ptr != nullptr) gameScene->detachGameObject(ptr);// 唯一性

        mng->setObjectName("mng");

        GameObject* duck[12];
        for(int j = 0;j < 12;j++)
        {
            duck[j] = new GameObject();
            auto duckPos = new Transform();
            auto duc = new WanderingDuck();
            duckPos->setPos(700 + 200 * cos(30 * j), 400 + 200 * sin(30 * j));
            duck[j]->addComponent(duckPos);
            duck[j]->addComponent(duc);
            gameScene->attachGameObject(duck[j]);
        }

        /* button */
        auto PVPbutton = new GameObject();
        auto btn = new Button();
        btn->index = 2; btn->mainWD = this; btn->gms = gameScene; btn->tmg = mng->getComponent<TitleManager>(); btn->str = " PVP 模式  ";
        auto bttrans = new Transform(QPointF(700, 470));
        PVPbutton->addComponent(bttrans);
        PVPbutton->addComponent(btn);
        gameScene->attachGameObject(PVPbutton);

        auto PVEbutton = new GameObject();
        auto btn2 = new Button();
        btn2->index = 1; btn2->mainWD = this; btn2->gms = gameScene; btn2->tmg = mng->getComponent<TitleManager>(); btn2->str = " PVE 模式  ";
        auto bttrans2 = new Transform(QPointF(1000, 470));
        PVEbutton->addComponent(bttrans2);
        PVEbutton->addComponent(btn2);
        gameScene->attachGameObject(PVEbutton);

        auto Tbutton = new GameObject();
        auto Tbtn = new Button();
        Tbtn->index = 3; Tbtn->mainWD = this; Tbtn->gms = gameScene; Tbtn->tmg = mng->getComponent<TitleManager>(); Tbtn->str = "     教程  ";
        auto Tbttrans = new Transform(QPointF(400, 470));
        Tbutton->addComponent(Tbttrans);
        Tbutton->addComponent(Tbtn);
        gameScene->attachGameObject(Tbutton);

        auto Obutton = new GameObject();
        auto Obtn = new Button();
        Obtn->index = -1; Obtn->mainWD = this; Obtn->gms = gameScene; Obtn->tmg = mng->getComponent<TitleManager>(); Obtn->str = " 退出游戏  ";
        auto Obttrans = new Transform(QPointF(700, 560));
        Obutton->addComponent(Obttrans);
        Obutton->addComponent(Obtn);
        gameScene->attachGameObject(Obutton);

    }
    else if(index == 1 || index == 2 || index == 3)// game, index 1 - PVE, index 2 - PVP, index 3 - tutorial;
    {
        gameScene->clearAll();

        auto Manager = new Mapmanager();
        Manager->gms = gameScene;
        Manager->mainWD = this;
        auto mng = new GameObject();
        if (index == 2 || index == 3) Manager->pvpEnabled = true;
        if (index == 3) Manager->isTutorial = true;

        GameObject* Player[4];
        int spawnpoint[4][2];// 初始化玩家和机器人的生成地点
        const int width = 64, height = 48;
        int map[15][20] = { 0 };

        GameObject* flr[15][20];// 背景

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

        if(index == 1 || index == 2)// 正常的随机化地图
        {
            for(int i = 1; i <= 13; i++)
                for(int j = 1;j <= 18;j++)
                {
                    int k = rand() % 10;
                    if(k>=1 && k<=4)
                        map[i][j] = k;
                }
        }
        if(index == 3)// 教程中使用的地图
        {
            for(int i = 5; i <= 9; i++)
                for(int j = 5; j <= 14; j++)
                    map[i][j] = 2 + rand() % 3;
        }


        for(int t = 0; t <= 1; t++)// 确保玩家周围什么也没有
            for(int i = spawnpoint[t][1] - 1; i <= spawnpoint[t][1]; i++)
                for(int j = spawnpoint[t][0] - 1; j <= spawnpoint[t][0] + 1; j++)
                    map[i][j] = 0;

        if(index == 1)// 如果有机器人，那么机器人周围也不应该有障碍物
        {
            spawnpoint[2][0] = 2, spawnpoint[2][1] = 1;
            spawnpoint[3][0] = 17, spawnpoint[3][1] = 1;

            for(int t = 2; t <= 3; t++)
                for(int i = spawnpoint[t][1]; i <= spawnpoint[t][1] + 1; i++)
                    for(int j = spawnpoint[t][0] - 1; j <= spawnpoint[t][0] + 1; j++)
                        map[i][j] = 0;
        }


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

        /* enemy */

        if(index == 1)// 机器人
        {
            auto trans = new Transform;
            Player[2] = new GameObject();
            ImageTransformBuilder()
                .setPos(QPointF(64 * spawnpoint[2][0] + 92, 48 * spawnpoint[2][1] + 48))
                .setImage(":/robot/images/rbt_d_3.png")
                .setAlignment(Qt::AlignCenter)
                .addToGameObject(Player[2]);
            trans->setPos(64 * spawnpoint[2][0] + 92, 48 * spawnpoint[2][1] + 48);
            auto controller3 = new UserController();
            controller3->playerID = 2;
            controller3->map = Manager;
            Player[2]->addComponent(trans);
            Player[2]->addComponent(new Physics());
            Player[2]->addComponent(controller3);
            gameScene->attachGameObject(Player[2]);

            Player[3] = new GameObject();
            ImageTransformBuilder()
                .setPos(QPointF(64 * spawnpoint[3][0] + 92, 48 * spawnpoint[3][1] + 48))
                .setImage(":/robot/images/rbt_d_3.png")
                .setAlignment(Qt::AlignCenter)
                .addToGameObject(Player[3]);
            trans->setPos(64 * spawnpoint[3][0] + 92, 48 * spawnpoint[3][1] + 48);
            auto controller4 = new UserController();
            controller4->playerID = 3;
            controller4->map = Manager;
            Player[3]->addComponent(trans);
            Player[3]->addComponent(new Physics());
            Player[3]->addComponent(controller4);
            gameScene->attachGameObject(Player[3]);
        }

        /* button */
        auto Bbutton = new GameObject();
        auto btnB = new Button();
        btnB->index = 0; btnB->mainWD = this; btnB->gms = gameScene; btnB->map = Manager; btnB->str = "返回主菜单  ";
        auto bttransB = new Transform(QPointF(192, 50));
        Bbutton->addComponent(bttransB);
        Bbutton->addComponent(btnB);
        gameScene->attachGameObject(Bbutton);

        auto Pbutton = new GameObject();
        auto btnP = new Button();
        btnP->index = -2; btnP->mainWD = this; btnP->gms = gameScene; btnP->map = Manager;
        auto bttransP = new Transform(QPointF(420, 50));
        Pbutton->addComponent(bttransP);
        Pbutton->addComponent(btnP);
        gameScene->attachGameObject(Pbutton);

        /* hooking */
        Manager->player1 = Player[0];
        Manager->player2 = Player[1];
        if (index == 1)
        {
            Manager->robot1 = Player[2];
            Manager->robot2 = Player[3];
        }

        auto trans = new Transform();
        trans->setPos(700, 400);// centered!
        mng->addComponent(trans);
        mng->addComponent(Manager);
        gameScene->attachGameObject(mng);

    //GameObject* ptr = gameScene->getGameObject("mng");
    //if (ptr != nullptr) gameScene->detachGameObject(ptr);// 唯一性

        mng->setObjectName("mng");
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

    loadScene(gameScene, 0);
}

MainWindow::~MainWindow() { delete ui; }
