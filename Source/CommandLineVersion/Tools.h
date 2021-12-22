#pragma once
#include<iostream>
#include<string>
#include<functional>
#include <windows.h>
#include<vector>

#include"Const.h"

using namespace std;

enum SituationType
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

struct Point
{
	int x, y;
	Point() :x(0), y(0) {}
	Point(int x, int y) :x(x), y(y) {}
};

class Gobang {
private:
	int chessBoard[BOARDSIZE][BOARDSIZE] = {};
	int ScoreMap[BOARDSIZE][BOARDSIZE] = {};

	bool isAIFirstChess; // judge whether need to execute the AIFirstPut func
	bool isAISecondChess; // judge whether need to execute the AISecondPut func

	Point nextStepForAI;
public:
	//construtor
	//set all chess as EMPTY
	Gobang();

	/*the board goes like this:
		  A  B  C ..  O
		1 ○ ● ○    ●
		2 ● ○ ●    ○
		. ..      ..
		. ..      ..
		15..      ..
	*/
	void printBoard();

	//for player to put a chess 
	//the chessType determines the color of chess
	//eg. playerTurn(BLACK,1,a);
	//return true if it's correctly put
	bool playerTurn(int chessType, int x, char y); // x=[0..14] y=[A..O] !!!!   

	//check whether the input is valid(not cross the border,eg)
	bool isValidInput(int x, char y);  // x=[0..14] y=[A..O]     
	bool isNotCrossingBorder(int x, int y); // x,y=[0..14] false if crossing the border

	//for AI
	void aiTurn(int chessType);
	
	vector<Point> generateNextStep(int chessType);
	void putChess(Point point,int chessType);
	void takeChess(Point point);
	int alphaBeta(int player, int alpha, int beta, int chessType, int depth = DEPTH);
	bool checkifFive(int chessType);
	void AIFirstPut(int chessType);
	void AISecondPut(int chessType);

	//judge the board,and return the winner number (BLACK or WHITE or EMPTY(no result) or draw)
	//especially whether the board is full
	//int judgeVictory(int chessType, int x, char y);  // x=[0..14] y=[A..O]   
	int judgeVictory();
	//if n=0,make n=1;
	int SolveZero(int n)
	{
		return n == 0 ? 1 : n;
	}

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

	bool getIsAIFirstChess() { return isAIFirstChess; }
	Point getAILastStep() { return nextStepForAI; }
};

int getIntValue(const function<bool(int)>& judgeFunc);
char getCharValue(const function<bool(char)>& judgeFunc);
