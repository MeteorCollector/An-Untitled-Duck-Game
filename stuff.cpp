#include "stuff.h"

Stuff::Stuff(): Component()
{}

void Stuff::onAttach()
{
    imgtrans = this->gameObject->getComponent<ImageTransform>();
    assert(imgtrans != nullptr);
    stage = 0;
    pace = 12;
}

void Stuff::onUpdate(float deltatime)
{
    count += deltatime;
    int pic = 0;
    if(index == 1 || index == 2) pic = int(count * pace) % 8;
    if(index == 3 || index == 4) pic = int(count * pace) % 6;
    if(pic == stage)
    {
    switch(index)
    {
    case 1:// boost
        if(pic == 0){ imgtrans->setImage(":/item/images/f_1.png"); stage++; }
        else if(pic == 1){ imgtrans->setImage(":/item/images/f_2.png"); stage++; }
        else if(pic == 2){ imgtrans->setImage(":/item/images/f_3.png"); stage++; }
        else if(pic == 3){ imgtrans->setImage(":/item/images/f_4.png"); stage++; }
        else if(pic == 4){ imgtrans->setImage(":/item/images/f_5.png"); stage++; }
        else if(pic == 5){ imgtrans->setImage(":/item/images/f_6.png"); stage++; }
        else if(pic == 6){ imgtrans->setImage(":/item/images/f_7.png"); stage++; }
        else if(pic == 7){ imgtrans->setImage(":/item/images/f_8.png"); stage = 0; }
        break;
    case 2:// bomb upgrade
        if(pic == 0){ imgtrans->setImage(":/item/images/b_1.png"); stage++; }
        else if(pic == 1){ imgtrans->setImage(":/item/images/b_2.png"); stage++; }
        else if(pic == 2){ imgtrans->setImage(":/item/images/b_3.png"); stage++; }
        else if(pic == 3){ imgtrans->setImage(":/item/images/b_4.png"); stage++; }
        else if(pic == 4){ imgtrans->setImage(":/item/images/b_5.png"); stage++; }
        else if(pic == 5){ imgtrans->setImage(":/item/images/b_6.png"); stage++; }
        else if(pic == 6){ imgtrans->setImage(":/item/images/b_7.png"); stage++; }
        else if(pic == 7){ imgtrans->setImage(":/item/images/b_8.png"); stage = 0; }
        break;
    case 3:// bomb sum
        if(pic == 0){ imgtrans->setImage(":/item/images/add_1.png"); stage++; }
        else if(pic == 1){ imgtrans->setImage(":/item/images/add_2.png"); stage++; }
        else if(pic == 2){ imgtrans->setImage(":/item/images/add_3.png"); stage++; }
        else if(pic == 3){ imgtrans->setImage(":/item/images/add_4.png"); stage++; }
        else if(pic == 4){ imgtrans->setImage(":/item/images/add_5.png"); stage++; }
        else if(pic == 5){ imgtrans->setImage(":/item/images/add_6.png"); stage = 0; }
        break;
    case 4:// bonb movable
        if(pic == 0){ imgtrans->setImage(":/item/images/m_1.png"); stage++; }
        else if(pic == 1){ imgtrans->setImage(":/item/images/m_2.png"); stage++; }
        else if(pic == 2){ imgtrans->setImage(":/item/images/m_3.png"); stage++; }
        else if(pic == 3){ imgtrans->setImage(":/item/images/m_4.png"); stage++; }
        else if(pic == 4){ imgtrans->setImage(":/item/images/m_5.png"); stage++; }
        else if(pic == 5){ imgtrans->setImage(":/item/images/m_6.png"); stage = 0; }
        break;
    default:
        break;
    }
    }

}
