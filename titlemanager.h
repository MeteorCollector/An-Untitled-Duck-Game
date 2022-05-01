#ifndef TITLEMANAGER_H
#define TITLEMANAGER_H
#include <gameobject.h>
#include <component.h>
#include <mainwindow.h>
#include <QMediaPlayer>
#include <transform.h>

class TitleManager: public Component
{
public:
    TitleManager();

    void onAttach() override;
    void onUpdate(float deltaTime) override;
    void showtitle();
    void hidetitle();
    void showPVEmenu();

    QMediaPlayer* mplr = nullptr;
    QAudioOutput* audioOutput = nullptr;

    GameScene* gms = nullptr;
    MainWindow* mainWD = nullptr;

private:
    QGraphicsTextItem *label = nullptr;
    QGraphicsRectItem *Rect = nullptr;
    QGraphicsRectItem *inRect = nullptr;
    QGraphicsTextItem *hint = nullptr;
    Transform *trans = nullptr;
    float myTimer = 0;
    float loopTime = 35;// 实际上有49秒
    float loopTimer = 0;
    int rate = 36;
};

#endif // TITLEMANAGER_H
