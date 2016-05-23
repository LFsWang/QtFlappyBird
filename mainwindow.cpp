#include "mainwindow.h"
#include <QMessageBox>
#include <QImageReader>
#include <QString>
#include <QtGlobal>
#include <QDebug>
#include <QLabel>
#include <QThread>

#include <Windows.h>

#include <algorithm>
#include <cassert>
#include <ctime>
#pragma comment(lib, "user32.lib")
static QImage doubleImage(const QImage &img)
{
    QImage   res(img.width()*2,img.height(),QImage::Format_RGB32);
    QPainter painter;
    painter.begin(&res);
    painter.drawImage(0,0,img);
    painter.drawImage(img.width(),0,img);
    painter.end();
    return res;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      bird(MainWindow::win_width,MainWindow::win_height,this),
      BoomGIF(":/Image/boom.gif"),
      Boom(this)

{
    //Load BackGround
    QImageReader Reader(tr(":/Image/Hope_Panorama_e.bmp"));
    QImage img = Reader.read();
    img = doubleImage(img);
    HomeBackGround = QPixmap::fromImage(img);
    if( HomeBackGround.isNull() )
    {
        QMessageBox::information(nullptr,tr("GG"),tr("image load fail"),QMessageBox::Ok);
        exit(0);
    }
    BGpos = 0;

    this->setFixedSize(win_width,win_height);	//視窗大小
    this->setWindowIcon(QIcon(":/Image/bird1.png"));
    this->setWindowTitle("Qt FlappyBird");
    window_mode = WindowsState::WINDWO_NONE;

    //創建水管
    for(OBSTACLE &p:pipe)
    {
        p.setParent(this);
    }

    { //buttons
        const int btn_h = 30;
        const int btn_d = 10;
        int starth = win_height/2;
        constexpr int xl=win_width/3;
        constexpr int w =win_width/3;

        btn_home_Start.setParent(this);
        btn_home_Start.setGeometry(xl,starth,w,btn_h);
        btn_home_Start.setText(QString::fromLocal8Bit("開始遊戲"));
        starth+=btn_h+btn_d;

        btn_home_Rank.setParent(this);
        btn_home_Rank.setGeometry(xl,starth,w,btn_h);
        btn_home_Rank.setText(QString::fromLocal8Bit("記分板"));
        starth+=btn_h+btn_d;

        btn_home_Close.setParent(this);
        btn_home_Close.setGeometry(xl,starth,w,btn_h);
        btn_home_Close.setText(QString::fromLocal8Bit("結束遊戲"));
    }

    //Script
    Boom.setMovie(&BoomGIF);

    connect(&btn_home_Start,SIGNAL(clicked()),this,SLOT(gameInit()));
    connect(&btn_home_Rank ,SIGNAL(clicked()),this,nullptr);
    connect(&btn_home_Close,SIGNAL(clicked()),this,SLOT(gameClose()));
    //btn_home_Close.setGeometry(0,0,100,100);
    btn_home_Close.show();



    //Init Clock
    connect(&GlobalClock,SIGNAL(timeout()),this,SLOT(updateBackGround()));
    GlobalClock.start(30);

    GameTimer.stop();
    connect(&GameTimer,SIGNAL(timeout()),this,SLOT(updateGame()));

    //遊戲初始模式
    updateBackGround();
    gameMainMenu();
}

void MainWindow::hide_all()
{
    Boom.hide();
    BoomGIF.stop();

    btn_home_Start.hide();
    btn_home_Rank.hide();
    btn_home_Close.hide();
    bird.hide();
    for(auto &c:pipe)
        c.hide();
}

void MainWindow::gameMainMenu()
{
    hide_all();
    btn_home_Start.show();
    btn_home_Rank.show();
    btn_home_Close.show();
}

void MainWindow::gameClose()
{
    QMessageBox::information(this,
                             QString::fromLocal8Bit("哈哈哈"),
                             QString::fromLocal8Bit("再見了殘酷的世界\nGoodbye Cruel World"),
                             QMessageBox::Ok);
    QApplication::quit();
}

void MainWindow::paintEvent(QPaintEvent *)		//繪圖事件, 用来產生背景
{
    //背景圖片
    QPainter painter(this);
    painter.drawPixmap(0,0,HomeBackGround,BGpos,0,BGpos+win_width,win_height);
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    isClickDown = true;
    qDebug()<<"press";
    // 在這裡設定按下滑鼠要觸發的功能
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 在這裡設定按下鍵盤要觸發的功能
}


void MainWindow::updateBackGround()
{
    if( !HomeBackGround.isNull() ){
        BGpos = (BGpos+GB_Delta) % (HomeBackGround.width()/2);
        this->update();
    }
}

void MainWindow::gameInit()
{
    hide_all();

    int wait_x = win_width*2;
    for(auto &c:pipe)
    {
        c.set(wait_x,100,100);
        wait_x += 500;
    }
    bird.reset();
    bird.show();

    //開場動畫!?
    for(auto &c:pipe)
        c.show();

    bird.move_release();
    GameTimer.start(30);
}

void MainWindow::updateGame()
{
    //鳥的運動
    if( (!bird.ishold()) && ( isClickDown || GetAsyncKeyState(VK_LBUTTON) ) ){
        bird.move_pos(30,-0.4);
    }
    else{
        bird.move_pos(30,0.19);
    }
    isClickDown = false;

    int max_x = 0;
    for(OBSTACLE &c:pipe)
    {
        c.moved(3);
        max_x = max(max_x,c.getX());
    }

    for(OBSTACLE &c:pipe)
    {
        if( c.getX() + c.width() < 0 )
        {
            c.set(max_x+500,200,200);
        }
    }
    if( isTouch() )
    {
        qDebug()<<"GG";
        GameTimer.stop();
        gameLose();
    }
}

void MainWindow::gameLose()
{
    Boom.setGeometry(bird.pos().x()-253/2,bird.pos().y()-253/2,253,233);
    Boom.raise();
    BoomGIF.start();
    Boom.show();
    QMessageBox::information(this,
                             QString::fromLocal8Bit("哈哈哈"),
                             QString::fromLocal8Bit("N00B"),
                             QMessageBox::Ok);
    gameMainMenu();
}

bool MainWindow::isTouch()
{
    int bird_l = bird.pos().x();
    int bird_r = bird.pos().x()+bird.width();
    int bird_u = bird.pos().y();
    int bird_d = bird.pos().y()+bird.height();
    for(OBSTACLE &c:pipe)
    {
        if( bird_r < c.pos().x() || c.pos().x()+c.width() < bird_l )continue;
        if( c.getGY() < bird_u && bird_d < c.getGY()+c.getGap() )continue;
        //TODO: 球體角落
        return true;
    }
    return false;
}
void MainWindow::gameRedy()
{

}
void MainWindow::gameStart()
{

}
