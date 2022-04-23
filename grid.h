#ifndef GRID_H
#define GRID_H
#include "component.h"
#include <imagetransform.h>

class Grid: public Component
{
public:
    Grid();
    int id, status;
    void onAttach() override;
    void broken();
private:
    ImageTransform *imgtrans;
};

#endif // GRID_H
