#include "grid.h"
#include <gameobject.h>
#include <transform.h>
#include <transformbuilder.h>
#include <imagetransform.h>
#include <QGraphicsRectItem>
#include <stuff.h>

Grid::Grid() : Component() {}

void Grid::onAttach() {
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    //auto trans = this->gameObject->getComponent<Transform>();
    if(imgtrans == nullptr) { return; }
    if(id == 1){ imgtrans->setImage(":/item/images/wall_n.png"); }
    else if(id >= 2){ imgtrans->setImage(":/item/images/wall_b.png"); }

    //auto Rect = new QGraphicsRectItem(trans);
    //Rect->setRect(QRectF(-32, -24, 64, 48));
}

void Grid::broken()
{    
    if(id >= 2){
        imgtrans->setImage(":/item/images/wall_r.png");
        if(id > 2)
        {
            int index = rand() % 4 + 1;
            stf = map->putStuff(i, j, index);
            map->tile[i][j] = - index;
            id = -index;
        }
        else
        {
            map->tile[i][j] = 0;
            id = 0;
        }
    }


}
