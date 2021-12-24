#ifndef GAME_H
#define GAME_H

#include"Const.h"

#include<iostream>
#include<string>
#include<functional>
#include<windows.h>
#include<vector>

using namespace std;

enum SituationType //Basic chess pattern
{
    BeFive,
    ActiveFour,
    RushFour,
    ActiveThree,
    SleepThree,
    ActiveTwo,
    SleepTwo,
    Other
};

struct Point //represent coordinate
{
    int x, y;
    Point() :x(0), y(0) {}
    Point(int x, int y) :x(x), y(y) {}
};

struct Gobang {
    //members
    int chessBoard[BOARDSIZE][BOARDSIZE] = {}; //board

    int gameType; // Game modes, including two-player battle and man-machine battle

    int playerFlag; // Indicates who is playing chess

    //members for ai
    bool isAIFirstChess; // judge whether need to execute the AIFirstPut func
    bool isAISecondChess; // judge whether need to execute the AISecondPut func
    Point nextStepForAI;

    int gameResult; //win or lose or draw

    int currentChessType;

    //methods
    //constructor
    Gobang();

    void switchCurrentChessType(){currentChessType=currentChessType==BLACK?WHITE:BLACK;}

    void startGame(int gameType);

    //for player to put a chess
    //the chessType determines the color of chess
    //return true if it's correctly put
    bool playerTurn(int chessType, int x, int y);

    bool isNotCrossingBorder(int x, int y); // x,y=[0..14] false if crossing the border

    //for AI
    void aiTurn(int chessType);

    //To produce the next possible move
    vector<Point> generateNextStep(int chessType);

    // put and take chess
    void putChess(Point point,int chessType);
    void takeChess(Point point);

    // AI core algorithm
    int alphaBeta(int player, int alpha, int beta, int chessType, int depth = DEPTH);

    // the first two steps of ai
    void AIFirstPut(int chessType);
    void AISecondPut(int chessType);

    // reset the first two steps
    void setisAIFirstChess(bool flag){isAIFirstChess=flag;}
    void setisAISecondChess(bool flag){isAIFirstChess=flag;}

    //judge the board,and return the winner number (BLACK or WHITE or EMPTY(no result) or draw)
    //especially whether the board is full
    int judgeVictory();

    //if n=0,make n=1;
    int SolveZero(int n){return n == 0 ? 1 : n;}

    // 查看是不是..
    SituationType JudgeBeFive(int x, int y, int dir, int chessType);
    SituationType JudegActiveFour(int x, int y, int dir, int chessType);
    SituationType JudgeRushFour(int x, int y, int dir, int chessType);
    SituationType JudgeActiveThreeSleepThree(int x, int y, int dir, int chessType);
    SituationType JudgeActvieTwoSleepTwo(int x, int y, int dir, int chessType);

    // 某种颜色棋子在某个方向上的棋型
    SituationType GetSituationType(int x, int y, int dir, int chessType);

    // 四个方向上对某个点某种颜色棋子进行估分
    int evaluate(int x, int y, int chessType);

    // 建立每个位置的分值表
    void SetScoreMap(int ScoreMap[BOARDSIZE][BOARDSIZE], int chessType);

    // 对整个棋盘打分
    int boardEvaluate(int chessBoard[BOARDSIZE][BOARDSIZE], int chessType);
};

#endif // GAME_H
