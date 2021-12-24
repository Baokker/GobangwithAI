#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
      setFixedSize(kBoardMargin * 2 + kBlockSize * 15, kBoardMargin * 2 + kBlockSize * 15 + kLabelSize);

      setMouseTracking(true);

      //Menu initialization, used to call different functions
      QMenu *gameMenu = menuBar()->addMenu(tr("Game")); // menuBar默认是存在的，直接加菜单就可以了

      QAction *actionPVP = new QAction("Person VS Person", this);
      connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
      gameMenu->addAction(actionPVP);

      QMenu* PlayFirst=gameMenu->addMenu(tr("Person VS Computer"));

      QAction *actionPVE_person = new QAction("Player first", this);
      connect(actionPVE_person, &QAction::triggered, this, [this](){
          initPVEGame(PVEwithPlayerFirst);
          });
      PlayFirst->addAction(actionPVE_person);

      QAction *actionPVE_ai = new QAction("Computer first", this);
      connect(actionPVE_ai, &QAction::triggered, this, [this](){
          initPVEGame(PVEwithAIFirst);
          });
      PlayFirst->addAction(actionPVE_ai);

      //label
      label=new QLabel(this);
      label->setGeometry(kBoardMargin,2*kBoardMargin+(BOARDSIZE-1)*kBlockSize,(BOARDSIZE-1)*kBlockSize,kLabelSize);
      label->setStyleSheet("QLabel { color : black; }");

      //begin
      initGame();
}

MainWindow::~MainWindow()
{
    if (gobang)
    {
        delete gobang;
        gobang = nullptr;
    }
}


void MainWindow::paintEvent(QPaintEvent *event)//for update
{
    QPainter painter(this);

    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    //    QPen pen; // 调整线条宽度
    //    pen.setWidth(2);
    //    painter.setPen(pen);

    //Draw board grids
    for (int i = 0; i < BOARDSIZE; i++)
    {
        painter.drawLine(kBoardMargin + kBlockSize * i  , kBoardMargin, kBoardMargin + kBlockSize * i, kBoardMargin+(BOARDSIZE-1)*kBlockSize );
        painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i,kBoardMargin+(BOARDSIZE-1)*kBlockSize , kBoardMargin + kBlockSize * i);
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    // 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow >= 0 && clickPosRow < BOARDSIZE &&
        clickPosCol >= 0 && clickPosCol < BOARDSIZE &&
        gobang->chessBoard[clickPosRow][clickPosCol] == 0)
    {
        if (gobang->currentChessType==WHITE)
            brush.setColor(Qt::white);
        else
            brush.setColor(Qt::black);
        painter.setBrush(brush);
        painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }

    // 绘制棋子
       for (int i = 0; i < BOARDSIZE; i++)
           for (int j = 0; j < BOARDSIZE; j++)
           {
               if (gobang->chessBoard[i][j] == WHITE)
               {
                   brush.setColor(Qt::white);
                   painter.setBrush(brush);
                   painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
               }
               else if (gobang->chessBoard[i][j] == BLACK)
               {
                   brush.setColor(Qt::black);
                   painter.setBrush(brush);
                   painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
               }
           }

       //label
       if (gobang->nextStepForAI.x >= 0)
       {
           string text="ai last step:"+to_string(gobang->nextStepForAI.x)+" "+to_string(gobang->nextStepForAI.y)+" (begin from 0)";
           label->setText(QString::fromStdString(text));
       }

     // 判断输赢
       if (clickPosRow >= 0 && clickPosRow < BOARDSIZE &&
               clickPosCol >= 0 && clickPosCol < BOARDSIZE &&
               (gobang->chessBoard[clickPosRow][clickPosCol] == BLACK ||
                   gobang->chessBoard[clickPosRow][clickPosCol] == WHITE))
       {
          int result= gobang->judgeVictory();
           QString str;
           if(result == BLACK || result == WHITE)
           {
               if(result == WHITE)
                    str="white player";
               else if(result==BLACK)
                   str="black player";

                 QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + " win!");
               if (btnValue == QMessageBox::Ok)
               {
                   if (gobang->gameType==PVP){
                       initPVPGame();
                   }
                   else{//pve
                       initPVEGame(gobang->gameType);
                   }
               }
           }
           else if(result==DRAW)
           {
                QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
                if (btnValue == QMessageBox::Ok)
                {
                    if (gobang->gameType==PVP){
                        initPVPGame();
                    }
                    else{//pve
                        initPVEGame(gobang->gameType);
                    }
                }
           }
       }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();

    // 棋盘边缘不能落子
    if (x >= kBoardMargin &&
            x < size().width() - kBoardMargin &&
            y > kBoardMargin &&
            y < kBoardMargin + (BOARDSIZE - 1) * kBlockSize)
    {
        clickPosRow = int(round(double(y-kBoardMargin) / kBlockSize));
        clickPosCol = int(round(double(x-kBoardMargin) / kBlockSize));
    }

    // 存了坐标后也要重绘
    update();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // 人下棋，并且不能抢机器的棋
    if (gobang->gameType == PVP || gobang->playerFlag == PLAYER)
    {
        chessOneByPerson();
        // 如果是人机模式，需要调用AI下棋
        if (gobang->gameType != PVP)
        {
            // 用定时器做一个延迟
            //QTimer::singleShot(kAIDelay, this, SLOT());
            chessOneByAI();
        }
    }

}

void MainWindow::chessOneByPerson()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && gobang->chessBoard[clickPosRow][clickPosCol] == EMPTY)
    {
        gobang->playerTurn(gobang->currentChessType,clickPosRow, clickPosCol);

        // 重绘
        update();

        //switch
        if(gobang->gameType != PVP)
            gobang->playerFlag=AI;
        gobang->switchCurrentChessType();
    }
}

void MainWindow::chessOneByAI()
{
    gobang->aiTurn(gobang->currentChessType);

    // 重绘
    update();

    //switch
    gobang->playerFlag=PLAYER;
    gobang->switchCurrentChessType();
}


void MainWindow::initGame()
{
    gobang = new Gobang;
    initPVPGame();
}

void MainWindow::initPVPGame()
{
    gobang->gameType = PVP;
    gobang->playerFlag = PLAYER;
    gobang->startGame(gobang->gameType);
    update();
}

void MainWindow::initPVEGame(int whoFirst)
{
     gobang->gameType=whoFirst;

     if (whoFirst==PVEwithPlayerFirst)
         gobang->playerFlag=PLAYER;
     else if (whoFirst==PVEwithAIFirst)
         gobang->playerFlag=AI;

     gobang->startGame(gobang->gameType);

     if (gobang->playerFlag == AI) chessOneByAI();
     update();
}
