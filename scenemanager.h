#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <component.h>
#include <mainwindow.h>


class SceneManager: public Component
{
public:
    SceneManager();
    void load(int index);
    MainWindow* mainWd = nullptr;
};

#endif // SCENEMANAGER_H
