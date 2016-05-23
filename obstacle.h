#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>

class OBSTACLE: public QWidget
{
    Q_OBJECT
public:
    explicit OBSTACLE(QWidget *parent = 0);
    explicit OBSTACLE(const OBSTACLE &)  = delete;
    explicit OBSTACLE(const OBSTACLE &&) = delete;
    ~OBSTACLE();

    int getX() const noexcept;
    int getGY() const noexcept;
    int getGap() const noexcept;
    void moved(int dx) noexcept;
    void set(int _X,int _gY,int _gH) noexcept;

private:
    int X;
    int gY;
    int gH;
    QPixmap pipeU;
    QPixmap pipeD;
protected:
    void paintEvent(QPaintEvent *);


};

#endif // OBSTACLE_H
