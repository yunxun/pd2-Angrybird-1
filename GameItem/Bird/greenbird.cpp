#include "greenbird.h"

GreenBird::GreenBird(float size_ratio, QPointF pos, QPixmap bird): Bird(size_ratio,pos,bird)
{

}

GreenBird::~GreenBird()
{


}

void GreenBird::special()
{

    g_body->SetLinearVelocity(b2Vec2(-80,-1));
}
