#ifndef YELLOWBIRD_H
#define YELLOWBIRD_H

#include "GameItem/Bird/bird.h"
/*
#define YELLOWBIRD_DENSITY 1600.0f
#define YELLOWBIRD_FRICTION 0.2f
#define YELLOWBIRD_RESTITUTION 0.2f
*/
class YellowBird: public Bird
{
public:
    YellowBird(float,QPointF,QPixmap);
    virtual ~YellowBird();
    virtual void special();
};

#endif // YELLOWBIRD_H
