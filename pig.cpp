#include "pig.h"
#include <QMovie>
#include<QLabel>

Pig::Pig(float size_ratio, b2Vec2 pos): GameItem(size_ratio)
{
    stamina *= 0.5;

    ++GameItem::pigCount;
    vulnerable=true;

    g_size = QSizeF(g_worldsize.height()*ratio,g_worldsize.height()*ratio);
    g_pixmap.setPixmap(QPixmap(":/image/Laughing_pig.gif").scaled(g_scene->height()*ratio,g_scene->height()*ratio));
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);

    QLabel *label = new QLabel();
    QMovie *movie = new QMovie(":/image/Laughing_pig.gif");
    label->setMovie(movie);
    label->setGeometry(0,0,0,0);
    movie->start();



    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set( pos.x,pos.y);
    bodydef.userData = this;
    g_body = g_world->CreateBody(&bodydef);

    b2CircleShape bodyshape;
    bodyshape.m_radius = g_worldsize.height()*ratio/2;

    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = PIG_DENSITY;
    fixturedef.friction = PIG_FRICTION;
    fixturedef.restitution = PIG_RESTITUTION;
    g_body->CreateFixture(&fixturedef);
    g_body->SetAngularDamping(1);
    g_body->SetLinearDamping(1);

    paint();
    g_scene->addItem(&g_pixmap);

    connect(g_timer, SIGNAL(timeout()), this, SLOT(paint()));
    connect(g_timer_check, SIGNAL(timeout()), this, SLOT(checkPos()));
}

Pig::~Pig()
{
    --GameItem::pigCount;
    GameItem::score+=prize;
}
