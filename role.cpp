#include "role.h"
#include <algorithm>


ROLE::ROLE(int _x, int _y, QWidget *parent) : QWidget(parent),win_x(_x),win_y(_y),
    final_x(win_x/2.0 - size)
{
    this->hide();
    this->setFixedSize(35,35);

    pic.load(":/Image/bird1.png");
    if( pic.isNull() ){
        QMessageBox::information(nullptr,tr("GG"),tr("bird load fail"),QMessageBox::Ok);
        exit(0);
    }
    reset();
}

ROLE::~ROLE()
{

}

void ROLE::reset() noexcept
{
    v=x=0.0;
    y = win_y/2.0 - size;
    holded = true;
    this->move(static_cast<int>(x),static_cast<int>(y));
}

void ROLE::move_release() noexcept
{
    holded = false;
}
bool ROLE::ishold() const noexcept
{
    return holded || x != final_x;
}
void ROLE::move_pos(int ms,double A)
{
    if( x < final_x )  // start not yet
    {
            x = std::min( x+ms/10.0 , final_x );
    }
    else if( !holded )
    {
        v += A;
        y += v;
        if( y >= win_y )y = win_y;
    }
    this->move(static_cast<int>(x),static_cast<int>(y));
}

void ROLE::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,size,size,pic);
}

