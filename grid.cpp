#include "grid.h"
#include <gameobject.h>
#include <transform.h>
#include <transformbuilder.h>
#include <imagetransform.h>
#include <QGraphicsRectItem>

Grid::Grid() : Component() {}

void Grid::onAttach() {
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    if(imgtrans == nullptr) { return; }
    if(id == 1){ imgtrans->setImage(":/item/images/wall_n.png"); }
    else if(id >= 2){ imgtrans->setImage(":/item/images/wall_b.png"); }
}

void Grid::broken()
{
    if(id >=2){ imgtrans->setImage(":/item/images/wall_r.png"); }
}
