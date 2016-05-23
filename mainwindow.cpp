#include "mainwindow.h"
#include <Windows.h>
#include <QMessageBox>
#include <QImageReader>
#include <QString>
#include <ctime>
#include <QtGlobal>
#include <QDebug>
#include <cassert>
#include <QLabel>
#include <QThread>
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
      bird(MainWindow::win_width,MainWindow::win_height,this)
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

    //Some btn setting
    //home
    //btn_home_Close.setDefault(true);

    {
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
    connect(&btn_home_Start,SIGNAL(clicked()),this,SLOT(gameInit()));
    connect(&btn_home_Rank ,SIGNAL(clicked()),this,nullptr);
    connect(&btn_home_Close,SIGNAL(clicked()),this,SLOT(gameClose()));
    //btn_home_Close.setGeometry(0,0,100,100);
    btn_home_Close.show();
    //創建水管
    for(int i=0;i<pipeCount;i++)
        pipe[i]=new OBSTACLE(this);

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
    btn_home_Start.hide();
    btn_home_Rank.hide();
    btn_home_Close.hide();
}

void MainWindow::gameMainMenu()
{

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
void MainWindow::createPipe()		//初始化水管，依序排在視窗外
{
    int startx=win_width+10;		//第一个水管的位置
    pipeXgap=200;		//水管水平間距
    int pipR;			//水管垂直位置隨機數
    qsrand(time(NULL));		//qrand是qt的隨機函數 用法和c的rand一樣

    for(int i=0;i<pipeCount;i++)
    {
        pipR=qrand()%200;

        pipe[i]->move(startx+i*pipeXgap,-200+pipR);
        lastPipe=i;		//很重要 記錄最後一個水管號碼 為後面的循環水管建立基礎
    }

    pipeTimer =new QTimer(this);	//pipeTimer 處理兩個動作
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(pipeAction()));
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(collisDete()));
    pipeTValue=8;

}
void MainWindow::createBird(){

    /*birds->move(60,250);
    //鳥的上下速度
    this->birdV_array[0]=-3;
    this->birdV_array[1]=-4;
    this->birdV_array[2]=-3;
    this->birdV_array[3]=-2;
    this->birdV_array[4]=-1;
    this->birdV_array[5]=-1;
    this->birdV_array[6]=-1;
    this->birdV_array[7]=0;
    this->birdV_array[8]=1;
    this->birdV_array[9]=1;
    this->birdV_array[10]=2;
    this->birdV_array[11]=2;
    this->birdV_array[12]=2;
    this->birdV_array[13]=3;
    this->birdV_array[14]=3;

    this->index_birdV=0;

    birdV=0;

    birdTimer=new QTimer(this);
    connect(birdTimer,SIGNAL(timeout()),this,SLOT(birdAction()));
    timedata=8;*/
}
void MainWindow::birdup(){
    /*index_birdV=0.0;
    birdV=birdV_array[int(index_birdV)];*/
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
    bird.reset();
    bird.show();
    GameTimer.start(30);
    //開場動畫!?
    bird.move_release();
}

void MainWindow::updateGame()
{
    //鳥的運動
    int c = GetAsyncKeyState(VK_LBUTTON);
    //qDebug()<<c<<((!bird.ishold()) && ( isClickDown || c ));
    if( (!bird.ishold()) && ( isClickDown || GetAsyncKeyState(VK_LBUTTON) ) ){
        bird.move_pos(30,-0.5);
    }
    else{
        bird.move_pos(30,0.19);
    }
    isClickDown = false;
    // 每次觸發這個function都會更改鳥的位置，x軸不變, y軸加上 birdV成為新的位置
    // XX->pos().x() and XX->pos().y() 是QWidget物件的函式，可以用來取得xy座標位置
    /*birds->move(birds->pos().x(),birds->pos().y()+birdV);


    if(index_birdV<14.0)
        index_birdV+=0.2;
    else
        index_birdV=14.0;

    birdV=birdV_array[int(index_birdV)];

    // 這邊做一個地板碰撞偵測
    if(birds->pos().y()+birds->height()>=win_height+10)
    {
        birds->move(birds->pos().x(),win_height-birds->height()+10); // XX->height可用來取得物件高度
        birdTimer->stop();
        gameLose();
    }*/
}
void MainWindow::pipeAction()
{
    //水管動畫，水管離開地圖後，重新回到右側接替lastPipe位置
    int pipR; // 一個隨機變數，讓每次水管的位置都不太一樣
    // 每次觸發這個函式，水管都會向左移動，如果最左邊的水管離開地圖範圍則移動到最右邊
    for(int i=0;i<pipeCount;i++)
    {
        pipe[i]->move(pipe[i]->pos().x()-1,pipe[i]->pos().y());
        if(pipe[i]->pos().x()<-100)
        {
            pipR=qrand()%200;
            pipe[i]->move(pipe[lastPipe]->pos().x()+pipeXgap,-200+pipR);
            lastPipe=i;
        }
    }
}
void MainWindow::collisDete()
{
    //水管碰撞偵測
    /*int birdRx=birds->pos().x()+30;
    int birdDy=birds->pos().y()+30;
    for(int i=0;i<pipeCount;i++)
    {
        if(birdRx>=pipe[i]->pos().x()&&birds->pos().x()<=pipe[i]->pos().x()+pipe[i]->width()-10) // XX->width可用來取得物件寬度
        {
            if(birds->pos().y() <= (pipe[i]->pos().y()+pipe[i]->getH1()) || birdDy >= (pipe[i]->pos().y()+pipe[i]->getH1()+pipe[i]->getGap()))
                //碰到水管遊戲結束
                gameLose();
        }
    }*/
}
void MainWindow::gameRedy()
{
    gamemod=redy;

    createBird();
    createPipe();
}
void MainWindow::gameLose()
{
    gamemod=lose;
    pipeTimer->stop();
}
void MainWindow::gameStart()
{
    gamemod=start;
    //birdTimer->start(timedata);
    pipeTimer->start(pipeTValue);
}
