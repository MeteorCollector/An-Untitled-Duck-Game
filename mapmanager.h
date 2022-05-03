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
    void putFlame(int i, int j, int dir, int level);
    GameObject* putStuff(int i, int j, int index);
    void onUpdate(float deltaTime) override;
    void onAttach() override;
    void load(int index);

    void deathUI();
    void victoryUI();
    void pvpEndUI(QString winner);
    bool pvpEnabled = false;

    int bombCnt[4] = { 0 };

private:
    bool exitingEnabled = true;
    int bgmid = 0;
    float myTimer = 0.0f;
    QGraphicsTextItem *label = nullptr;
    QGraphicsRectItem *Rect = nullptr;
    Transform *trans = nullptr;
};
#endif // MAPMANAGER_H
