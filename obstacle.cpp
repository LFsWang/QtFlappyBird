#include "obstacle.h"
#include <QMessageBox>
#include <cstdlib>
OBSTACLE::OBSTACLE(QWidget *parent) : QWidget(parent)
{
    this->hide();
    this->setFixedSize(70,600);
    pipeU.load(":/Image/pipe1.png");
    pipeD.load(":/Image/pipe2.png");
    if(pipeU.isNull()||pipeD.isNull())
    {
        QMessageBox::information(nullptr,tr("GG"),tr("bird load fail"),QMessageBox::Ok);
        exit(0);
    }
}

OBSTACLE::~OBSTACLE()
{

}

void OBSTACLE::moved(int dx) noexcept
{
    X-=dx;
    this->move(X,0);
}

void OBSTACLE::set(int _X,int _gY,int _gH) noexcept
{
    X=_X;
    gY=_gY;
    gH=_gH;
    this->move(_X,0);
}

void OBSTACLE::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0    ,pipeU,0,pipeU.height()-gY,pipeU.width(),pipeU.height());
    painter.drawPixmap(0,gY+gH,pipeD);
   /* pix.load(":/Image/pipe2.png");
    painter.drawPixmap(0,300+Gap,70,300,pix);*/
}

int OBSTACLE::getX() const noexcept //水管長度
{
    return X;
}

int OBSTACLE::getGY() const noexcept
{
    return gY;
}

int OBSTACLE::getGap() const noexcept// 上下水管間隔
{
    return gH;
}

