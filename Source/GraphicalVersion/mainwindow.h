#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    // 绘制
    void paintEvent(QPaintEvent *event);
    // 监听鼠标移动情况，方便落子
    void mouseMoveEvent(QMouseEvent *event);
     // 实际落子
    void mouseReleaseEvent(QMouseEvent *event);

 private:
     Gobang *gobang; // 游戏指针
     //GameType game_type; // 存储游戏类型
     int clickPosRow, clickPosCol; // 存储将点击的位置
     void initGame();
     void checkGame(int y, int x);

     QLabel* label;

private slots:
     void chessOneByPerson(); // 人执行
     void chessOneByAI(); // AI下棋
     void initPVPGame();
     void initPVEGame(int whoFirst);
};
#endif // MAINWINDOW_H
