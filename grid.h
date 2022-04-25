#ifndef GRID_H
#define GRID_H
#include "component.h"
#include <imagetransform.h>
#include <mapmanager.h>
#include <stuff.h>

class Grid: public Component
{
public:
    Grid();
    int id, status, i, j;
    void onAttach() override;
    void broken();
    Mapmanager* map = nullptr;
    GameObject* stf = nullptr;
private:
    ImageTransform *imgtrans;
};

#endif // GRID_H
