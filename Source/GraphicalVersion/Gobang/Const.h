#ifndef CONST_H
#define CONST_H

//Constants required for graphical interface
const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kPosDelta = 20; // 鼠标点击的模糊距离上限
const int kAIDelay = 700; // AI下棋的思考时间

//Types of different moves on the board
const int BLACK = 1;
const int WHITE = -1;
const int EMPTY = 0;

// Types of different ending
// BLACK -> black win
// WHITE -> white win
const int DRAW = 2;
const int NOWINNER = 3;

//Board size
const int BOARDSIZE = 15;

//role
const int AI = 0;
const int PLAYER = 1;

//Game Type
const int PVEwithPlayerFirst=0;
const int PVP=1;
const int PVEwithAIFirst=2;

//depth
//change DEPTH here to change the recursive depth of the alphabeta algorithm
const int DEPTH = 6;

//bool
const bool AIFirst=false;
const bool PlayerFirst=true;

//Basic chess pattern
//成五
const int BE_FIVE = 10000000;
//活四
const int ACTIVIE_FOUR = 50000;
//冲四
const int RUSH_FOUR = 3000;
//活三
const int ACTIVIE_THREE = 3000;
//眠三
const int SLEEP_THREE = 200;
//活二
const int ACTIVE_TWO = 200;
//眠二
const int SLEEP_TWO = 10;
//其他
const int OTHER = 1;

//direction
const int updown = 0; //上下方向
const int leftright = 1; //左右方向
const int leftoblique = 2;//左斜线
const int rightoblique = 3;//右斜线

#endif // CONST_H
