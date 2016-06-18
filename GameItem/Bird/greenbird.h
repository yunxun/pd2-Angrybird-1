#ifndef GREENBIRD_H
#define GREENBIRD_H

#include "GameItem/Bird/bird.h"
/*
#define GREENBIRD_DENSITY 850.0f
#define GREENBIRD_FRICTION 0.7f
#define GREENBIRD_RESTITUTION 0.1f
*/

class GreenBird : public Bird
{
public:
    GreenBird(float,QPointF,QPixmap);
    virtual ~GreenBird();
    virtual void special();
};

#endif // GREENBIRD_H
