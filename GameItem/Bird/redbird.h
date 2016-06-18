#ifndef REDBIRD_H
#define REDBIRD_H

#include "GameItem/Bird/bird.h"
/*
#define BIRD_DENSITY 1200.0f
#define BIRD_FRICTION 0.5f
#define BIRD_RESTITUTION 0.3f
*/
class RedBird: public Bird
{
public:
    RedBird(float,QPointF,QPixmap);
    virtual ~RedBird();
    virtual void special();
};

#endif // REDBIRD_H
