#ifndef STUFF_H
#define STUFF_H

#include <component.h>
#include <transformbuilder.h>

class Stuff: public Component
{
public:
    Stuff();
    int index;// 1 - boost; 2 - bomb upgrade; 3 - bomb sum; 4 - bomb move. The index in tile array is negative.
    void onUpdate(float deltatime) override;
    void onAttach() override;
private:
    int pace, stage;
    float count;
    ImageTransform* imgtrans = nullptr;
};

#endif // STUFF_H
