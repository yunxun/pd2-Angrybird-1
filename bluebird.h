#ifndef BLUEBIRD_H
#define BLUEBIRD_H

#include "GameItem/Bird/bird.h"
/*
#define BLUEBIRD_DENSITY 850.0f
#define BLUEBIRD_FRICTION 0.7f
#define BLUEBIRD_RESTITUTION 0.1f
*/
class BlueBird: public Bird
{
public:
    BlueBird(float,QPointF,QPixmap);
    virtual ~BlueBird();
    virtual void special();
};

#endif // BLUEBIRD_H
