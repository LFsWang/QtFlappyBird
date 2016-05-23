#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QMessageBox>
#include <cmath>

class ROLE : public QWidget
{
    Q_OBJECT

public:
    ROLE(int _x,int _y,QWidget *parent = 0);
    explicit ROLE(const ROLE &)  = delete;
    explicit ROLE(const ROLE &&) = delete;
    void reset() noexcept;
    ~ROLE();
    void move_pos(int ms,double F);
    void move_release() noexcept;
    static const int size = 35;
    bool ishold() const noexcept;
protected:
    void paintEvent(QPaintEvent *);

private:
    QPixmap pic;
    const int win_x;
    const int win_y;
    const double final_x;
    double x;
    double y;
    double v; // y-dir speed
    bool holded;

};

#endif // ROLE_H
