#include"Tools.h"

int getIntValue(const function<bool(int)>& judgeFunc) {//for freely customizing the int value you want
	int value;
	while (true)
	{
		cin >> value;
		if (cin.fail()) {
			cerr << "input error\n";
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else if (!judgeFunc(value)) {
			cerr << "dont meet the command\n";
		}
		else
			break;
	}

	return value;
}

char getCharValue(const function<bool(char)>& judgeFunc) {//for freely customizing the char value you want
	char value;
	while (true)
	{
		cin >> value;
		if (cin.fail()) {
			cerr << "input error\n";
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else if (!judgeFunc(value)) {
			cerr << "dont meet the command\n";
		}
		else
			break;
	}

	return value;
}

Gobang::Gobang()
{
	for (int i = 0; i < BOARDSIZE; i++)
		for (int j = 0; j < BOARDSIZE; j++) {
			chessBoard[i][j] = EMPTY;
			ScoreMap[i][j] = 0;
		}

	isAIFirstChess = isAISecondChess = true;
}

void Gobang::printBoard()
{
	//cout << "    ";
	//for (char ch = 'A'; ch <= 'O'; ch++)
	//	cout << ch << "  ";
	//for (int row = 0; row < 15; row++)
	//{
	//	cout << endl << row + 1 << "  ";
	//	if (row + 1 < 10)
	//		cout << ' ';
	//	for (int list = 0; list < 15; list++)
	//	{
	//		if (chessBoard[row][list] == 0)
	//			cout << "   ";
	//		else if (chessBoard[row][list] == BLACK)
	//			cout << "○ ";
	//		else if (chessBoard[row][list] == WHITE)
	//			cout << "● ";
	//	}
	//}
	//cout << endl;

	cout << "     ";
	for (char ch = 'A'; ch <= 'O'; ch++)
		cout << ch << "  ";
	cout << endl;
	for (int i = 0; i < BOARDSIZE * 3 + 10; i++)
		cout << "-";
	for (int row = 0; row < 15; row++)
	{
		cout << endl << row + 1 << "  ";
		if (row + 1 < 10)
			cout << ' ';

		cout << "|";

		for (int list = 0; list < 15; list++)
		{
			if (chessBoard[row][list] == 0)
				cout << "  ";
			else if (chessBoard[row][list] == BLACK)
				cout << "○";
			else if (chessBoard[row][list] == WHITE)
				cout << "●";

			cout << "|";
		}

		cout << endl;

		for (int i = 0; i < BOARDSIZE * 3 + 10; i++)
			cout << "-";
	}

	cout << endl;
}

bool Gobang::playerTurn(int chessType, int x, char y)
{
	if (isValidInput(x, y))
	{
		if (chessType == BLACK)
			chessBoard[x][y - 'A'] = BLACK;
		else if (chessType == WHITE)
			chessBoard[x][y - 'A'] = WHITE;
		return true;
	}
	else
	{
		cout << "输入有误，请重新输入！\n";
		return false;
	}
}

int Gobang::boardEvaluate(int chessBoard[BOARDSIZE][BOARDSIZE], int chessType)
{
	int sum = 0;
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			if (chessBoard[i][j] == chessType)
				sum += evaluate(i, j, chessType);
			else if (chessBoard[i][j] == -chessType)
				sum -= evaluate(i, j, -chessType);
		}
	}
	return sum;
}


bool Gobang::isValidInput(int x, char y)
{
	return isNotCrossingBorder(x, int(y - 'A')) && chessBoard[x][int(y - 'A')] == EMPTY;;
}

bool Gobang::isNotCrossingBorder(int x, int y)
{
	if (x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE)
		return true;
	else
		return false;
}

void Gobang::aiTurn(int chessType)
{
	if (isAIFirstChess) {
		AIFirstPut(chessType);
		return;
	}

	if (isAISecondChess) {
		AISecondPut(chessType);
		return;
	}

	alphaBeta(AI, INT_MIN, INT_MAX, chessType);

	putChess(nextStepForAI, chessType);
}


vector<Point> Gobang::generateNextStep(int chessType)
{
	vector<Point> save;
	int count = 1;
	int i, j, num;
	while (1)
	{
		for (i = 0; i < BOARDSIZE; i++)
		{
			for (j = 0; j < BOARDSIZE; j++)
			{
				if (chessBoard[i][j] != EMPTY) continue;

				for (int dir = 0; dir < 4; dir++)
				{
					if (count == 1 && JudgeBeFive(i, j, dir, chessType) != Other)
						save.push_back(Point(i, j));
					else if (count == 2 && JudgeBeFive(i, j, dir, -chessType) != Other)
						save.push_back(Point(i, j));
					else if (count == 3 && JudegActiveFour(i, j, dir, chessType) != Other)
						save.push_back(Point(i, j));
					else if (count == 4 && JudegActiveFour(i, j, dir, -chessType) != Other)
						save.push_back(Point(i, j));
					else if (count == 5 && (JudgeRushFour(i, j, dir, chessType) != Other || JudgeActiveThreeSleepThree(i, j, dir, chessType) == ActiveThree))
						save.push_back(Point(i, j));
					else if (count == 6 && (JudgeRushFour(i, j, dir, -chessType) != Other || JudgeActiveThreeSleepThree(i, j, dir, -chessType) == ActiveThree))
						save.push_back(Point(i, j));
					//else if (count == 7 && (JudgeActiveThreeSleepThree(i, j, dir, chessType) == SleepThree || JudgeActvieTwoSleepTwo(i, j, dir, chessType) == ActiveTwo))
					//	save.push_back(Point(i, j));
					//else if (count == 8 && (JudgeActiveThreeSleepThree(i, j, dir, -chessType) == SleepThree || JudgeActvieTwoSleepTwo(i, j, dir, -chessType) == ActiveTwo))
					//	save.push_back(Point(i, j));
					else if (dir == 0 && count == 7)
					{
						num = 0;
						for (int m = i - 1; m <= i + 1; m++)
							for (int n = j - 1; n <= j + 1; n++)
								if ((m != i || n != j) && m > 0 && m < BOARDSIZE && n>0 && n < BOARDSIZE && chessBoard[m][n] != EMPTY)
									num++;
						if (num > 1)
							save.push_back(Point(i, j));

					}
				}
			}
		}
		if (save.size() != 0)
			return save;
		else
			count++;
	}
}

void Gobang::putChess(Point point,int chessType)
{
	chessBoard[point.x][point.y] = chessType;
}

void Gobang::takeChess(Point point)
{
	chessBoard[point.x][point.y] = EMPTY;
}


int Gobang::alphaBeta(int player, int alpha, int beta, int aiChessType, int depth)
{
	if (judgeVictory() != NOWINNER) {
		return boardEvaluate(chessBoard, aiChessType);
	}

	if (depth == 0) { // 另外应该还有一个终止条件，就是已经连成五个子了
		return boardEvaluate(chessBoard, aiChessType);
	}

	int value;

	if (player == AI) {
		auto nextStepArray = generateNextStep(aiChessType);

		for (auto nextStep : nextStepArray) {
			putChess(nextStep, aiChessType);

			value = alphaBeta(PLAYER, alpha, beta, aiChessType, depth - 1);
			
			if (value > alpha && depth == DEPTH) {
				nextStepForAI = nextStep;
			}

			alpha = max(alpha, value);

			takeChess(nextStep);

			if (beta <= alpha)
				break;

			if (alpha >= BE_FIVE) break;
		}

		return alpha;
	}
	else { // (player == PLAYER) 
		auto nextStepArray = generateNextStep(-aiChessType);

		for (auto nextStep : nextStepArray) {
			putChess(nextStep, -aiChessType);

			value = alphaBeta(AI, alpha, beta, aiChessType, depth - 1);

			beta = min(beta, value);

			takeChess(nextStep);

			if (beta <= alpha)
				break;

			if (beta <= -BE_FIVE) break;
		}

		return beta;
	}
}

bool Gobang::checkifFive(int chessType)
{
	int countLine = 0, countRow = 0, countLeftOblique = 0, countRightOblique = 0;

	//line
	for (int i = 0; i < BOARDSIZE; i++) {
		countRow = 0;
		for (int j = 0; j < BOARDSIZE; j++) {
			if (chessBoard[i][j] == chessType) {
				countRow++;
			}
			else countRow = 0;

			if (countRow == 5) return true;
		}
	}

	//row
	for (int j = 0; j < BOARDSIZE; j++) {
		countLine = 0;
		for (int i = 0; i < BOARDSIZE; i++) {
			if (chessBoard[i][j] == chessType) {
				countLine++;
			}
			else countLine = 0;

			if (countLine == 5) return true;
		}
	}

	//lefttop to rightdown
	/*
		\
		 \
		  \
	*/
	for (int i = BOARDSIZE - 5; i >= 0; i--) {
		countRightOblique = 0;
		for (int j = 0,m = i; j < BOARDSIZE; j++,m++) {
			if (chessBoard[m][j] == chessType) {
				countRightOblique++;
			}
			else countLine = 0;

			if (countRightOblique == 5) return true;
		}
	}

	//lefttop to rightdown
	/*
		   /	
		  /
		 /
	*/
	for (int i = 4; i < BOARDSIZE; i++) {
		countLeftOblique = 0;
		for (int j = 0, m = i; j < BOARDSIZE; j++, m--) {
			if (chessBoard[m][j] == chessType) {
				countLeftOblique++;
			}
			else countLine = 0;

			if (countLeftOblique == 5) return true;
		}
	}
	return false;
}

void Gobang::AIFirstPut(int chessType)
{
	isAIFirstChess = false;

	nextStepForAI = Point(BOARDSIZE / 2, BOARDSIZE / 2);

	int count = 0;
	for (int i = 0; i < BOARDSIZE; i++)
		for (int j = 0; j < BOARDSIZE; j++) {
			if (chessBoard[i][j] != EMPTY)
				count++;
		}

	// no chess
	// put it in the center
	if (count==0) {
		putChess(Point(BOARDSIZE / 2, BOARDSIZE / 2), chessType);
		return;
	}

	// there is a black chess on the board
	// put the white chess on the center if it's not occupied
	// otherwise a random position next to the center
	if (count == 1) {
		if (chessBoard[BOARDSIZE / 2][BOARDSIZE / 2] == EMPTY) {
			putChess(Point(BOARDSIZE / 2, BOARDSIZE / 2), chessType);
			return;
		}
		else {
			srand(time(NULL));
			int deltaX = 0, deltaY = 0;
			while (!deltaX || !deltaY) {
				deltaX = rand() % 3 - 1;
				deltaY = rand() % 3 - 1;
			}

			putChess(Point(BOARDSIZE / 2 + deltaX, BOARDSIZE / 2 + deltaY), chessType);
			return;
		}
	}

	return;
}

void Gobang::AISecondPut(int chessType)
{
	isAISecondChess = false;

	int centerNum = 0;

	for (int m = BOARDSIZE / 2 - 1; m <= BOARDSIZE / 2 + 1; m++)
		for (int n = BOARDSIZE / 2 - 1; n <= BOARDSIZE / 2 + 1; n++)
			if (chessBoard[m][n] != EMPTY)
				centerNum++;

	// ai firstlt puts the black chess on (7,7),but the player just put it far from the black chess
	// ai's first chess is bound to be on the center,so we just put it on one of (6,6)(8,8)(6,8)(8,6)
	if (centerNum == 1 && chessBoard[BOARDSIZE / 2][BOARDSIZE / 2] == BLACK) {
		srand(time(NULL));
		int x, y, choice = rand() % 4;
		switch (choice) {
		case 0:
			x = y = 6;
			break;
		case 1:
			x = y = 8;
			break;
		case 2:
			x = 6; y = 8;
			break;
		case 3:
			x = 8; y = 6;
			break;
		default:
			break;
		}

		nextStepForAI = Point(x, y); // record the path
		putChess(nextStepForAI, chessType);
		return;
	}
	// player first but he doesn't put the chess on the center
	// there must remain a white chess around the corner ,just so so
	else if (centerNum == 1) {
		srand(time(NULL));
		int x, y;
		do {
			x = rand() % 3 - 1 + BOARDSIZE / 2;
			y = rand() % 3 - 1 + BOARDSIZE / 2;
		} while (chessBoard[x][y] != EMPTY);

		nextStepForAI = Point(x, y);
		putChess(nextStepForAI, chessType);
		return;
	}
	else { //count >= 2,there must be 1 chess that has 2 chesses around it
		alphaBeta(AI, INT_MIN, INT_MAX, chessType);
		putChess(nextStepForAI, chessType);
		return;
	}
}

//int Gobang::judgeVictory(int chessType, int x, char y)  // x=[0..14] y=[A..O]   
//{
//	int line = y - 'A';
//	int row = x;
//	if (row >= 4 && chessBoard[row - 1][line] + chessBoard[row - 2][line]
//		+ chessBoard[row - 3][line] + chessBoard[row - 4][line] == 4 * chessType)
//		return chessType;
//	else if (row <= 10 && chessBoard[row + 1][line] + chessBoard[row + 2][line]
//		+ chessBoard[row + 3][line] + chessBoard[row + 4][line] == 4 * chessType)
//		return chessType;
//	else if (line >= 4 && chessBoard[row][line - 1] + chessBoard[row][line - 2]
//		+ chessBoard[row][line - 3] + chessBoard[row][line - 4] == 4 * chessType)
//		return chessType;
//	else if (line <= 10 && chessBoard[row][line + 1] + chessBoard[row][line + 2]
//		+ chessBoard[row][line + 3] + chessBoard[row][line + 4] == 4 * chessType)
//		return chessType;
//	else if (row >= 4 && line >= 4 && chessBoard[row - 1][line - 1] + chessBoard[row - 2][line - 2]
//		+ chessBoard[row - 3][line - 3] + chessBoard[row - 4][line - 4] == 4 * chessType)
//		return chessType;
//	else if (row <= 10 && line <= 10 && chessBoard[row + 1][line + 1] + chessBoard[row + 2][line + 2]
//		+ chessBoard[row + 3][line + 3] + chessBoard[row + 4][line + 4] == 4 * chessType)
//		return chessType;
//	else if (row >= 4 && line <= 10 && chessBoard[row - 1][line + 1] + chessBoard[row - 2][line + 2]
//		+ chessBoard[row - 3][line + 3] + chessBoard[row - 4][line + 4] == 4 * chessType)
//		return chessType;
//	else if (row <= 10 && line >= 4 && chessBoard[row + 1][line - 1] + chessBoard[row + 2][line - 2]
//		+ chessBoard[row + 3][line - 3] + chessBoard[row + 4][line - 4] == 4 * chessType)
//		return chessType;
//	return 0;
//}

int Gobang::judgeVictory()
{
	int sum = 0;
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			if (chessBoard[i][j] == BLACK)
			{
				for (int dir = 0; dir < 4; dir++)
					if (JudgeBeFive(i, j, dir, BLACK)==BeFive)
						return BLACK;
			}
			else if (chessBoard[i][j] == WHITE)
			{
				for (int dir = 0; dir < 4; dir++)
					if (JudgeBeFive(i, j, dir, WHITE) == BeFive)
						return WHITE;
			}
			if (chessBoard[i][j] != EMPTY)
				sum++;
		}
	}
	if (sum == BOARDSIZE * BOARDSIZE)
		return DRAW;
	return NOWINNER;
}

SituationType Gobang::JudgeBeFive(int x, int y, int dir, int chessType)
{
	for (int i = 1; i >= -4; i--)
	{
		if (i == 0)
			continue;
		else
		{
			int delta1 = i;
			int delta2 = SolveZero(i + 1);
			int delta3 = SolveZero(delta2 + 1);
			int delta4 = SolveZero(delta3 + 1);
			if (dir == updown) //上下方向
			{
				if (isNotCrossingBorder(x, y + delta1) && isNotCrossingBorder(x, y + delta4) &&
					chessBoard[x][y + delta1] == chessType &&
					chessBoard[x][y + delta2] == chessType &&
					chessBoard[x][y + delta3] == chessType &&
					chessBoard[x][y + delta4] == chessType)
					return BeFive;
			}
			else if (dir == leftright)//左右方向
			{
				if (isNotCrossingBorder(x + delta1, y) && isNotCrossingBorder(x + delta4, y) &&
					chessBoard[x + delta1][y] == chessType &&
					chessBoard[x + delta2][y] == chessType &&
					chessBoard[x + delta3][y] == chessType &&
					chessBoard[x + delta4][y] == chessType)
					return BeFive;
			}
			else if (dir == leftoblique)//左斜线
			{
				if (isNotCrossingBorder(x + delta1, y + delta1) && isNotCrossingBorder(x + delta4, y + delta4) &&
					chessBoard[x + delta1][y + delta1] == chessType &&
					chessBoard[x + delta2][y + delta2] == chessType &&
					chessBoard[x + delta3][y + delta3] == chessType &&
					chessBoard[x + delta4][y + delta4] == chessType)
					return BeFive;
			}
			else if (dir == rightoblique)//右斜线
			{
				if (isNotCrossingBorder(x + delta1, y - delta1) && isNotCrossingBorder(x + delta4, y - delta4) &&
					chessBoard[x + delta1][y - delta1] == chessType &&
					chessBoard[x + delta2][y - delta2] == chessType &&
					chessBoard[x + delta3][y - delta3] == chessType &&
					chessBoard[x + delta4][y - delta4] == chessType)
					return BeFive;
			}
		}
	}
	return Other;
}

SituationType Gobang::JudegActiveFour(int x, int y, int dir, int chessType)
{
	for (int i = -1; i >= -4; i--)
	{
		if (i == 0)
			continue;
		else
		{
			int delta1 = i;
			int delta2 = SolveZero(i + 1);
			int delta3 = SolveZero(delta2 + 1);
			int delta4 = SolveZero(delta3 + 1);
			int delta5 = SolveZero(delta4 + 1);
			if (dir == updown)
			{
				if (isNotCrossingBorder(x, y + delta1) && isNotCrossingBorder(x, y + delta5) &&
					chessBoard[x][y + delta1] == 0 &&
					chessBoard[x][y + delta2] == chessType &&
					chessBoard[x][y + delta3] == chessType &&
					chessBoard[x][y + delta4] == chessType &&
					chessBoard[x][y + delta5] == 0)
					return ActiveFour;
			}
			else if (dir == leftright)
			{
				if (isNotCrossingBorder(x + delta1, y) && isNotCrossingBorder(x + delta5, y) &&
					chessBoard[x + delta1][y] == 0 &&
					chessBoard[x + delta2][y] == chessType &&
					chessBoard[x + delta3][y] == chessType &&
					chessBoard[x + delta4][y] == chessType &&
					chessBoard[x + delta5][y] == 0)
					return ActiveFour;
			}
			else if (dir == leftoblique)
			{
				if (isNotCrossingBorder(x + delta1, y + delta1) && isNotCrossingBorder(x + delta5, y + delta5) &&
					chessBoard[x + delta1][y + delta1] == 0 &&
					chessBoard[x + delta2][y + delta2] == chessType &&
					chessBoard[x + delta3][y + delta3] == chessType &&
					chessBoard[x + delta4][y + delta4] == chessType &&
					chessBoard[x + delta5][y + delta5] == 0)
					return ActiveFour;
			}
			else if (dir == rightoblique)
			{
				if (isNotCrossingBorder(x + delta1, y - delta1) && isNotCrossingBorder(x + delta5, y - delta5) &&
					chessBoard[x + delta1][y - delta1] == 0 &&
					chessBoard[x + delta2][y - delta2] == chessType &&
					chessBoard[x + delta3][y - delta3] == chessType &&
					chessBoard[x + delta4][y - delta4] == chessType &&
					chessBoard[x + delta5][y - delta5] == 0)
					return ActiveFour;
			}
		}
	}
	return Other;
}

SituationType Gobang::JudgeRushFour(int x, int y, int dir, int chessType)
{
	for (int i = 1; i >= -4; i--)
	{
		if (i == 0)
			continue;
		else
		{
			int delta1 = i;
			int delta2 = SolveZero(i + 1);
			int delta3 = SolveZero(delta2 + 1);
			int delta4 = SolveZero(delta3 + 1);
			if (dir == updown)
			{
				if (isNotCrossingBorder(x, y + delta1) && isNotCrossingBorder(x, y + delta4) &&
					chessBoard[x][y + delta1] +
					chessBoard[x][y + delta2] +
					chessBoard[x][y + delta3] +
					chessBoard[x][y + delta4] == chessType * 3)
					return RushFour;
			}
			else if (dir == leftright)
			{
				if (isNotCrossingBorder(x + delta1, y) && isNotCrossingBorder(x + delta4, y) &&
					chessBoard[x + delta1][y] == +
					chessBoard[x + delta2][y] == +
					chessBoard[x + delta3][y] == +
					chessBoard[x + delta4][y] == chessType * 3)
					return RushFour;
			}
			else if (dir == leftoblique)
			{
				if (isNotCrossingBorder(x + delta1, y + delta1) && isNotCrossingBorder(x + delta4, y + delta4) &&
					chessBoard[x + delta1][y + delta1] +
					chessBoard[x + delta2][y + delta2] +
					chessBoard[x + delta3][y + delta3] +
					chessBoard[x + delta4][y + delta4] == 3 * chessType)
					return RushFour;
			}
			else if (dir == rightoblique)
			{
				if (isNotCrossingBorder(x + delta1, y - delta1) && isNotCrossingBorder(x + delta4, y - delta4) &&
					chessBoard[x + delta1][y - delta1] +
					chessBoard[x + delta2][y - delta2] +
					chessBoard[x + delta3][y - delta3] +
					chessBoard[x + delta4][y - delta4] == 3 * chessType)
					return RushFour;
			}
		}
	}
	return Other;
}

SituationType Gobang::JudgeActiveThreeSleepThree(int x, int y, int dir, int chessType)
{
	for (int i = -1; i >= -4; i--)
	{
		int delta1 = i;
		int delta2 = SolveZero(i + 1);
		int delta3 = SolveZero(delta2 + 1);
		int delta4 = SolveZero(delta3 + 1);
		int delta5 = SolveZero(delta4 + 1);
		if (dir == updown)
		{
			if (isNotCrossingBorder(x, y + delta1) && isNotCrossingBorder(x, y + delta5))
				if (chessBoard[x][y + delta1] == 0 &&
					chessBoard[x][y + delta5] == 0 &&
					chessBoard[x][y + delta2] +
					chessBoard[x][y + delta3] +
					chessBoard[x][y + delta4] == chessType * 2)
					return ActiveThree;
				else if (chessBoard[x][y + delta1] +
					chessBoard[x][y + delta5] == -chessType &&
					chessBoard[x][y + delta2] +
					chessBoard[x][y + delta3] +
					chessBoard[x][y + delta4] == chessType * 2)
					return SleepThree;
		}
		else if (dir == leftright)
		{
			if (isNotCrossingBorder(x + delta1, y) && isNotCrossingBorder(x + delta5, y))
				if (chessBoard[x + delta1][y] == 0 &&
					chessBoard[x + delta2][y] +
					chessBoard[x + delta3][y] +
					chessBoard[x + delta4][y] == chessType * 2 &&
					chessBoard[x + delta5][y] == 0)
					return ActiveThree;
				else if (chessBoard[x + delta1][y] +
					chessBoard[x + delta5][y] == -chessType &&
					chessBoard[x + delta2][y] +
					chessBoard[x + delta3][y] +
					chessBoard[x + delta4][y] == chessType * 2)
					return SleepThree;
		}
		else if (dir == leftoblique)
		{
			if (isNotCrossingBorder(x + delta1, y + delta1) && isNotCrossingBorder(x + delta5, y + delta5))
				if (chessBoard[x + delta1][y + delta1] == 0 &&
					chessBoard[x + delta2][y + delta2] +
					chessBoard[x + delta3][y + delta3] +
					chessBoard[x + delta4][y + delta4] == chessType * 2 &&
					chessBoard[x + delta5][y + delta5] == 0)
					return ActiveThree;
				else if (chessBoard[x + delta1][y + delta1] +
					chessBoard[x + delta5][y + delta5] == -chessType &&
					chessBoard[x + delta2][y + delta2] +
					chessBoard[x + delta3][y + delta3] +
					chessBoard[x + delta4][y + delta4] == chessType * 2)
					return SleepThree;
		}
		else if (dir == rightoblique)
		{
			if (isNotCrossingBorder(x + delta1, y - delta1) && isNotCrossingBorder(x + delta5, y - delta5))
				if (chessBoard[x + delta1][y - delta1] == 0 &&
					chessBoard[x + delta2][y - delta2] +
					chessBoard[x + delta3][y - delta3] +
					chessBoard[x + delta4][y - delta4] == chessType * 2 &&
					chessBoard[x + delta5][y - delta5] == 0)
					return ActiveThree;
				else if (chessBoard[x + delta1][y - delta1] +
					chessBoard[x + delta5][y - delta5] == -chessType &&
					chessBoard[x + delta2][y - delta2] +
					chessBoard[x + delta3][y - delta3] +
					chessBoard[x + delta4][y - delta4] == chessType * 2)
					return SleepThree;
		}
	}
	return Other;
}

SituationType Gobang::JudgeActvieTwoSleepTwo(int x, int y, int dir, int chessType)
{
	for (int i = -1; i >= -4; i--)
	{
		int delta1 = i;
		int delta2 = SolveZero(i + 1);
		int delta3 = SolveZero(delta2 + 1);
		int delta4 = SolveZero(delta3 + 1);
		int delta5 = SolveZero(delta4 + 1);
		if (dir == leftright)
		{
			if (isNotCrossingBorder(x, y + delta1) && isNotCrossingBorder(x, y + delta5))
				if (chessBoard[x][y + delta1] == 0 &&
					chessBoard[x][y + delta5] == 0 &&
					chessBoard[x][y + delta2] +
					chessBoard[x][y + delta3] +
					chessBoard[x][y + delta4] == chessType &&
					chessBoard[x][y + delta2] *
					chessBoard[x][y + delta3] *
					chessBoard[x][y + delta4] == 0)
					return ActiveTwo;
				else if ((i == -1 && chessBoard[x][y + delta1] == -chessType && chessBoard[x][y + delta5] == 0 ||
					i == -4 && chessBoard[x][y + delta5] == -chessType && chessBoard[x][y + delta1] == 0) &&
					chessBoard[x][y + delta2] +
					chessBoard[x][y + delta3] +
					chessBoard[x][y + delta4] == chessType &&
					chessBoard[x][y + delta2] *
					chessBoard[x][y + delta3] *
					chessBoard[x][y + delta4] == 0)
					return SleepTwo;
		}
		else if (dir == updown)
		{
			if (isNotCrossingBorder(x + delta1, y) && isNotCrossingBorder(x + delta5, y))
				if (chessBoard[x + delta1][y] == 0 &&
					chessBoard[x + delta2][y] +
					chessBoard[x + delta3][y] +
					chessBoard[x + delta4][y] == chessType &&
					chessBoard[x + delta5][y] == 0 &&
					chessBoard[x + delta2][y] *
					chessBoard[x + delta3][y] *
					chessBoard[x + delta4][y] == 0)
					return ActiveTwo;
				else if ((i == -1 && chessBoard[x + delta1][y] == -chessType && chessBoard[x + delta5][y] == 0 ||
					i == -4 && chessBoard[x + delta5][y] == -chessType && chessBoard[x + delta1][y] == 0) &&
					chessBoard[x + delta2][y] +
					chessBoard[x + delta3][y] +
					chessBoard[x + delta4][y] == chessType &&
					chessBoard[x + delta2][y] *
					chessBoard[x + delta3][y] *
					chessBoard[x + delta4][y] == 0)
					return SleepTwo;
		}
		else if (dir == rightoblique)
		{
			if (isNotCrossingBorder(x + delta1, y + delta1) && isNotCrossingBorder(x + delta5, y + delta5))
				if (chessBoard[x + delta1][y + delta1] == 0 &&
					chessBoard[x + delta2][y + delta2] +
					chessBoard[x + delta3][y + delta3] +
					chessBoard[x + delta4][y + delta4] == chessType &&
					chessBoard[x + delta5][y + delta5] == 0 &&
					chessBoard[x + delta2][y + delta2] *
					chessBoard[x + delta3][y + delta3] *
					chessBoard[x + delta4][y + delta4] == 0)
					return ActiveTwo;
				else if ((i == -1 && chessBoard[x + delta1][y + delta1] == -chessType && chessBoard[x + delta5][y + delta5] == 0 ||
					i == -4 && chessBoard[x + delta5][y + delta5] == -chessType && chessBoard[x + delta1][y + delta1] == 0) &&
					chessBoard[x + delta2][y + delta2] +
					chessBoard[x + delta3][y + delta3] +
					chessBoard[x + delta4][y + delta4] == chessType &&
					chessBoard[x + delta2][y + delta2] *
					chessBoard[x + delta3][y + delta3] *
					chessBoard[x + delta4][y + delta4] == 0)
					return SleepTwo;
		}
		else if (dir == leftoblique)
		{
			if (isNotCrossingBorder(x + delta1, y - delta1) && isNotCrossingBorder(x + delta5, y - delta5))
				if (chessBoard[x + delta1][y - delta1] == 0 &&
					chessBoard[x + delta2][y - delta2] +
					chessBoard[x + delta3][y - delta3] +
					chessBoard[x + delta4][y - delta4] == chessType &&
					chessBoard[x + delta5][y - delta5] == 0 &&
					chessBoard[x + delta2][y - delta2] *
					chessBoard[x + delta3][y - delta3] *
					chessBoard[x + delta4][y - delta4] == 0)
					return ActiveTwo;
				else if ((i == -1 && chessBoard[x + delta1][y - delta1] == -chessType && chessBoard[x + delta5][y - delta5] == 0 ||
					i == -4 && chessBoard[x + delta5][y - delta5] == -chessType && chessBoard[x + delta1][y - delta1] == 0) &&
					chessBoard[x + delta2][y - delta2] +
					chessBoard[x + delta3][y - delta3] +
					chessBoard[x + delta4][y - delta4] == chessType &&
					chessBoard[x + delta2][y - delta2] *
					chessBoard[x + delta3][y - delta3] *
					chessBoard[x + delta4][y - delta4] == 0)
					return SleepTwo;
		}
	}
	return Other;
}

SituationType Gobang::GetSituationType(int x, int y, int dir, int chessType)
{
	SituationType type = JudgeBeFive(x, y, dir, chessType);
	if (type == Other)
	{
		type = JudegActiveFour(x, y, dir, chessType);
		if (type == Other)
		{
			type = JudgeRushFour(x, y, dir, chessType);
			if (type == Other)
			{
				type = JudgeActiveThreeSleepThree(x, y, dir, chessType);
				if (type == Other)
				{
					//return type = JudgeActvieTwoSleepTwo(x, y, dir, chessType);
					type = JudgeActvieTwoSleepTwo(x, y, dir, chessType);
					return type;
				}
				else
					return type;
			}
			else
				return type;
		}
		else
			return type;
	}
	else
		return type;
}

int Gobang::evaluate(int x, int y, int chessType)
{
	int sum = 0;
	//四个方向估值
	for (int i = 0; i < 4; i++)
	{
		SituationType type = GetSituationType(x, y, i, chessType);
		switch (type)
		{
		case BeFive:
			sum += BE_FIVE;
			break;
		case ActiveFour:
			sum += ACTIVIE_FOUR;
			break;
		case RushFour:
			sum += RUSH_FOUR;
			break;
		case ActiveThree:
			sum += ACTIVIE_THREE;
			break;
		case SleepThree:
			sum += SLEEP_THREE;
			break;
		case ActiveTwo:
			sum += ACTIVE_TWO;
			break;
		case SleepTwo:
			sum += SLEEP_TWO;
			break;
		default:
			sum += OTHER;
			break;
		}
	}
	return sum;
}

void Gobang::SetScoreMap(int ScoreMap[BOARDSIZE][BOARDSIZE], int chessType)
{
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			if (chessBoard[i][j] == 0)
				ScoreMap[i][j] = evaluate(i, j, chessType);
		}
	}
}
