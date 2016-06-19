#include "obstacle.h"

Obstacle::Obstacle(b2Vec2 pos, QSizeF wh): GameItem(0.01)
{
    //stamina *= 0.3;

    g_size = QSizeF(g_worldsize.height()*ratio*wh.width(),g_worldsize.height()*ratio*wh.height());
    //g_pixmap.setPixmap(barrier.scaled(g_scene->height()*ratio,g_scene->height()*ratio));
    //g_pixmap.setPos(pos.x()-g_pixmap.pixmap().width()/2,pos.y()-g_pixmap.pixmap().height()/2);
    g_pixmap.setPixmap(QPixmap(":/image/Wood_Block.png").scaled(g_scene->height()*ratio*wh.width(),g_scene->height()*ratio*wh.height()));
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);

    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(pos.x,pos.y);
    bodydef.userData = this;
    g_body = g_world->CreateBody(&bodydef);

    b2PolygonShape bodyshape;
    bodyshape.SetAsBox(g_size.width()/2,g_size.height()/2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = OBSTACLE_DENSITY;
    fixturedef.friction = OBSTACLE_FRICTION;
    fixturedef.restitution = OBSTACLE_RESTITUTION;
    g_body->CreateFixture(&fixturedef);
    g_body->SetAngularDamping(1);
    g_body->SetLinearDamping(1);

    paint();
    g_scene->addItem(&g_pixmap);

    vulnerable=true;

    connect(g_timer,SIGNAL(timeout()),this,SLOT(paint()));
    connect(g_timer_check,SIGNAL(timeout()),this,SLOT(checkPos()));
}

Obstacle::~Obstacle()
{
    GameItem::score+=500;
}

