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
    QMediaPlaylist* plist = nullptr;

    GameScene* gms = nullptr;
    MainWindow* mainWD = nullptr;

    int tile[15][20] = { 0 };
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

    int bombCnt[4] = { 0 };
};
#endif // MAPMANAGER_H
