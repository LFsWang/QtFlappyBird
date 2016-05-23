#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMouseEvent>
#include <QPushButton>

#include <random>
#include <vector>

#include <role.h>
#include <obstacle.h>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);    //繪製背景圖
    void mousePressEvent(QMouseEvent *event);	//滑鼠點下事件
    void keyPressEvent(QKeyEvent *event);	//鍵盤事件

private slots:
    void updateGame();			//
    void updateBackGround();    //循環背景
    void gameClose();           //關閉遊戲Egg
    void gameInit();            //開始遊戲

private:
    void gameMainMenu();        //Game init
    void gameLose();			 //遊戲結束
    void gameStart();			 //遊戲開始
    void gameRedy();             //遊戲預備
    bool isTouch(); 			//碰撞偵測
    void hide_all();

    std::mt19937 mt_rand;
    QTimer GlobalClock;
    QPixmap HomeBackGround;
    static const int GB_Delta = 1;
    int BGpos;

    bool isClickDown;
    //Main Menu Btn
    QPushButton btn_home_Start;
    QPushButton btn_home_Rank;
    QPushButton btn_home_Close;

    ROLE bird;             // 建立一隻角色 - 鳥

    static const int pipeCount=5;   // pipe 數量
    OBSTACLE pipe[pipeCount];       // 建立水管

    QTimer GameTimer;
    double timedata;			// birdTimer interval

    enum class WindowsState{
        WINDWO_NONE,
        WINDOW_HOME
    };
    WindowsState window_mode;


    struct Level{
        int distance;           ///< 回合長度
        int speed;              ///< 移動速度
        std::pair<int,int> Gap; ///< 洞口大小
        std::string name;       ///< 關卡提示
    };
    std::vector<Level> LevelTable;
    int level;


    static const int win_width =380;     // 視窗寬度
    static const int win_height=450;     // 視窗高度
};

#endif // MAINWINDOW_H
