#pragma once

constexpr int BLACK = 1;
constexpr int WHITE = -1;
constexpr int EMPTY = 0;
constexpr int DRAW = 2;
constexpr int BOARDSIZE = 15;
const int NOWINNER = 3;

//AI
const int AI = 0;
const int PLAYER = 1;
const int DEPTH = 10;

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
const int updown = 0; //上下方向
const int leftright = 1; //左右方向
const int leftoblique = 2;//左斜线
const int rightoblique = 3;//右斜线
