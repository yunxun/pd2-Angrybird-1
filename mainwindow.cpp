#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NUM_Bird 5

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    qsrand(QTime::currentTime().second());

    //add BGM
    BGM = new QMediaPlayer(this);
    BGM->setMedia(QUrl("qrc:/music/BGM.mp3"));


    //add aim sound
    Aim_sound = new QMediaPlayer(this);
    Aim_sound->setMedia(QUrl("qrc:/music/Aim.mp3"));


    //add wood sound
    Wood_colli_sound = new QMediaPlayer(this);
    Wood_colli_sound->setMedia(QUrl("qrc:/music/wood_colli.mp3"));
    Wood_destroy_sound = new QMediaPlayer(this);
    Wood_destroy_sound->setMedia(QUrl("qrc:/music/wood_destroy.mp3"));
    Wood_rolling_sound = new QMediaPlayer(this);
    Wood_rolling_sound->setMedia(QUrl("qrc:/sound/wood_rolling.mp3"));

    //add rock sound
    Rock_colli_sound = new QMediaPlayer(this);
    Rock_colli_sound->setMedia(QUrl("qrc:/music/rock_colli.mp3"));
    Rock_destroy_sound = new QMediaPlayer(this);
    Rock_destroy_sound->setMedia(QUrl("qrc:/music/rock_destroy.mp3"));
    Rock_rolling_sound = new QMediaPlayer(this);
    Rock_rolling_sound->setMedia(QUrl("qrc:/sound/rock_rolling.mp3"));


    //add pig sound
    pig_delete_SOUND = new QMediaPlayer(this);
    pig_delete_SOUND->setMedia(QUrl("qrc:/music/pig_delete.mp3"));
    pig_yell_sound = new QMediaPlayer(this);
    pig_yell_sound->setMedia(QUrl("qrc:/sound/pig_yell.mp3"));

    //add bird sound
    Fire_BIRD_SOUND = new QMediaPlayer(this);
    Fire_BIRD_SOUND->setMedia(QUrl("qrc:/music/fire.mp3"));
    Bird_Falling_Sound = new QMediaPlayer(this);
    Bird_Falling_Sound->setMedia(QUrl("qrc:/music/falling.mp3"));
    Bird_Bouncing_Sound = new QMediaPlayer(this);
    Bird_Bouncing_Sound->setMedia(QUrl("qrc:/music/bird_bounce.mp3"));
    Bird_Red_Yell = new QMediaPlayer(this);
    Bird_Red_Yell->setMedia(QUrl("qrc:/music/redbird_yell.mp3"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    initGame();
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    QMouseEvent* mouseEvent = (QMouseEvent*)event;
    if(gameEnded || birdie==NULL || GameItem::invulnerability || QRectF(0,0,150,300).contains(mouseEvent->pos())) return false;
    if(!drag && event->type()==QEvent::MouseButtonPress)
    {
        //music
        Aim_sound->play();
        if(birdie->launched)
        {
                Bird_Red_Yell->play();
                birdie->special();
                birdie=NULL;
        }
        else if(dragBox.contains(mouseEvent->pos()) && nextRound<0)
        {
            Bird_Bouncing_Sound->play();
            drag = true;
            start = mouseEvent->pos();//QCursor::pos();
            dx=0;//
            dy=0;
        }
    }
    if(!drag || nextRound>=0) return false;
    if(event->type()==QEvent::MouseMove)
    {
        dx = mouseEvent->pos().x()-start.x();
        dy = mouseEvent->pos().y()-start.y();
        dl = pow(pow(dx,2)+pow(dy,2),0.5);
        dl = (dl<1? 1: dl);
        dx = dx*(dl>V_MAX? V_MAX: dl)/dl;
        dy = dy*(dl>V_MAX? V_MAX: dl)/dl;
        birdie->setBirdPos(origin,dx,dy);

    }
    if(event->type()==QEvent::MouseButtonRelease)
    {
        Fire_BIRD_SOUND->play();
        drag = false;
        nextRound=4;
        birdie->launch(b2Vec2((float)-dx*0.4,(float)dy*0.4));
        disconnect(timer_check,SIGNAL(timeout()),this,SLOT(checkStatus()));
        connect(timer_check,SIGNAL(timeout()),this,SLOT(checkStable()));
        ui->label_Remain->setText("x "+QString::number(--GameItem::birdCount));
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    emit quitGame();
}

void MainWindow::initGame()
{
    //music
    BGM->play();
    connect(BGM,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(replayBGM()));


    timer = new QTimer();
    timer->start(1000/FPS);
    timer_check = new QTimer();
    timer_check->start(500);

    drag = false;
    gameEnded = false;
    nextRound = 1;

    world = new b2World(b2Vec2(0.0f, -20.0f));
    world->SetContactListener(new GameListener());
    scene = new QGraphicsScene(0,0,width(),height());
    scene->setBackgroundBrush(QImage(":/image/back.jpg").scaled(width(),height()));
    ui->graphicsView->setScene(scene);

    birds = {1,2,3,4,5};
    GameItem::initGameItem(QSizeF(WORLD_W,WORLD_H),size(),world,scene,timer,timer_check,&itemList,birds.size());
    origin = QPointF(width()*0.1, height()*0.75);

    QGraphicsPixmapItem *catapult = new QGraphicsPixmapItem();
    catapult->setPixmap(QPixmap(":/image/aim.png").scaled(height()*0.1,height()*0.2));
    catapult->setPos(origin.x()-catapult->pixmap().width()/2,origin.y()-25);
    scene->addItem(catapult);

    for(float k = 0.025; k<=1 ; k+=0.025)
    {
        itemList.push_back(new Land(0,b2Vec2(WORLD_W*0.5,0),QSizeF(WORLD_W,WORLD_H*0.25),true));
    }

    for(float i=0.5; i<=0.81; i+=0.1)
    {
        for(float j=0.2; j<=0.71; j+=0.15)
        {
            itemList.push_back(new Pig(0.07,b2Vec2(WORLD_W*i,WORLD_H*j)));
            itemList.push_back(new Obstacle(b2Vec2(WORLD_W*i,WORLD_H*(j+0.07)),QSizeF(17, 2)));
            itemList.push_back(new Obstacle(b2Vec2(WORLD_W*(i-0.04),WORLD_H*j),QSizeF(2.5,11)));
            itemList.push_back(new Obstacle(b2Vec2(WORLD_W*(i+0.04),WORLD_H*j),QSizeF(2.5,11)));
        }
    }
    itemList.push_back(new Pig(0.07,b2Vec2(WORLD_W*0.4,WORLD_H*(0.5+0.02))));
    itemList.push_back(new Obstacle(b2Vec2(WORLD_W*0.4,WORLD_H*(0.3+0.02)),QSizeF(17, 2)));
    itemList.push_back(new Obstacle(b2Vec2(WORLD_W*0.4,WORLD_H*(0.2)),QSizeF(2.5, 11)));


    connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    connect(timer,SIGNAL(timeout()),this,SLOT(clearWasted()));
    connect(timer_check,SIGNAL(timeout()),this,SLOT(checkStable()));

    ui->label_Remain->setText("x "+QString::number(GameItem::birdCount));
    ui->label_Result->clear();

}

void MainWindow::order()
{
    drag=false;
    checkStatus();
    if(gameEnded || nextRound>0 || birds.size()==0) return;

    int bird_type;
    bird_type = rand()%NUM_Bird;

    switch(bird_type)
    {
        case 0 : birdie = new RedBird(0.07,origin,QPixmap(":/image/red.png"));      break;
        case 1 : birdie = new YellowBird(0.07,origin,QPixmap(":/image/yellow.png"));break;
        case 2 : birdie = new BlueBird(0.05,origin,QPixmap(":/image/blue.png"));   break;
        case 3 : birdie = new BigBird(0.1,origin,QPixmap(":/image/bigred.png"));     break;
        case 4 : birdie = new GreenBird(0.07,origin,QPixmap(":/image/green.png")); break;
        default: birdie = NULL; break;
    }

    birds.removeFirst();
    itemList.push_back(birdie);
    QPointF temp = QPointF(birdie->g_pixmap.pixmap().width()/2,birdie->g_pixmap.pixmap().height()/2);
    dragBox = QRectF(origin-temp,origin+temp);
}

void MainWindow::nextFrame()
{
    world->Step(1.0/FPS,10,2);
    scene->update();
    ui->label_Score->setText("SCORE ： ");
    ui->lcdNumber->display(((int)GameItem::score));
}

void MainWindow::clearWasted()
{
    for(int i=0; i<itemList.size(); ++i)
    {
        if(itemList[i]->wasted)
        {
            delete itemList[i];
            itemList.erase(itemList.begin()+i);
            --i;
        }
    }
}

void MainWindow::checkStable()
{
    b2Body *bodyList = world->GetBodyList();
    while(bodyList!=NULL)
    {
        if(bodyList->GetLinearVelocity().Length()>1)        {
            nextRound=4;
            return;
        }
        bodyList = bodyList->GetNext();
    }
    if(--nextRound<0)
    {
        GameItem::invulnerability=false;
        disconnect(timer_check,SIGNAL(timeout()),this,SLOT(checkStable()));
        connect(timer_check,SIGNAL(timeout()),this,SLOT(checkStatus()));
        order();
    }
}

void MainWindow::checkStatus()
{
    if(GameItem::birdCount==0)
    {
        gameEnded=true;
        ui->label_Result->setText("TRY AGAIN");
    }
    if(GameItem::pigCount==0)
    {
        gameEnded=true;
        ui->label_Result->setText("YOU WIN!");
        GameItem::score+=5000*GameItem::birdCount;
        disconnect(timer_check,SIGNAL(timeout()),this,SLOT(checkStatus()));
    }
}

void MainWindow::on_exit_clicked()
{
    close();
}

void MainWindow::on_restart_clicked()
{


    gameEnded=true;
    delete timer;
    delete timer_check;
    for(int i=0; i<itemList.size(); ++i)
        delete itemList[i];
    itemList.clear();
    delete world;
    delete scene;
    initGame();
}

void MainWindow::replayBGM()
{
    BGM->play();
}
