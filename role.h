#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <math.h>

class ROLE : public QWidget
{
    Q_OBJECT
public:
    explicit ROLE(QWidget *parent = 0);
    explicit ROLE(const ROLE &)  = delete;
    explicit ROLE(const ROLE &&) = delete;
    ~ROLE();

protected:
    void paintEvent(QPaintEvent *);

private:
    QString src;

};

#endif // ROLE_H
