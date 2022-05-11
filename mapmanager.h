#ifndef MAPMANAGER_H
#define MAPMANAGER_H
#include <gameobject.h>
#include <component.h>
#include <mainwindow.h>
#include <stuff.h>
#include <QMediaPlayer>

class Mapmanager: public Component
{
public:
    Mapmanager();

    QMediaPlayer* mplr = nullptr;
    QAudioOutput* audioOutput = nullptr;

    GameScene* gms = nullptr;
    MainWindow* mainWD = nullptr;

    int tile[15][20] = { 0 };
    int bmbdata[15][20] = { 0 };
    GameObject* arr[15][20];
    GameObject* bmb[15][20];

    GameObject* player1 = nullptr;
    GameObject* player2 = nullptr;
    GameObject* robot1 = nullptr;
    GameObject* robot2 = nullptr;

    void putBomb(int i, int j, int level, int countdown, int index);
    void putFlame(int i, int j, int dir, int level, int index);
    GameObject* putStuff(int i, int j, int index);
    void onUpdate(float deltaTime) override;
    void onAttach() override;
    void load(int index);

    void deathUI(int score = 114514);
    void victoryUI(int score = 114514);
    void pvpEndUI(QString winner, int score = 114514);
    void showPauseUI();
    void hidePauseUI();

    bool pvpEnabled = false;
    bool isTutorial = false;

    int bombCnt[4] = { 0 };

private:

    void showTutorial();

    bool exitingEnabled = true;
    int bgmid = 0;
    float myTimer = 0.0f;
    QGraphicsTextItem *label = nullptr;
    QGraphicsRectItem *Rect = nullptr;
    QGraphicsRectItem *lRect = nullptr;
    QGraphicsRectItem *rRect = nullptr;
    QGraphicsRectItem *Panel = nullptr;
    Transform *trans = nullptr;
};
#endif // MAPMANAGER_H
