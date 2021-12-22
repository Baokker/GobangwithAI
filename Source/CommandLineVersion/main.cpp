#include"Tools.h"

int main() {
	Gobang gobang;
	cout << "switch mode:\n"
		<<"1-player vs player 2-player vs AI\n";
	const int PLAYERvsPLAYER = 1, PLAYERvsAI = 2;

	int choice = getIntValue([&](int value)->bool {return value == PLAYERvsPLAYER || value == PLAYERvsAI; }); //lambda function C11

	switch (choice)
	{
	case PLAYERvsPLAYER: {
		int currentChessType = BLACK;
		int x; char y;
		int result;

		while (true) {
			//print the chess
			system("cls"); //clean the screen
			gobang.printBoard();

			//put the chess
			cout << "now it's " << (currentChessType == BLACK ? "Black" : "White") << " chess\n"
				<< "please input like \"1 a\"\n";

			do {
				x = getIntValue([&](int value)->bool {return value > 0 && value <= BOARDSIZE; }); //[1.. 15]
				y = getCharValue([&](char value)->bool {return (value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z'); });	
			} while (!gobang.playerTurn(currentChessType, x - 1, toupper(y)));//x [0..14]

			//judge
			result = gobang.judgeVictory();
			if (result == BLACK || result == WHITE) {
				system("cls"); //clean the screen
				gobang.printBoard();
				cout << (result == BLACK ? "Black" : "White") << " is the winner\n";
				break;
			}
			else if (result == DRAW) {
				system("cls"); //clean the screen
				gobang.printBoard();
				cout << "draw!!\n";
				break;
			}

			//switch chessType
			currentChessType = currentChessType == BLACK ? WHITE : BLACK;
		}
		break;
	}

	case PLAYERvsAI: {
		int currentChessType = BLACK;
		int x; char y;
		int result;
		bool isPlayer;
		
		Point aiTurnRecord;

		cout << "please choose to be BLACK or WHITE (BLACK first as default)\n"
		<< "1-BLACK 2-WHITE\n";
		int isFirst = getIntValue([&](int value)->bool {return value == 1 || value == 2; });
		isPlayer = isFirst == 1 ? true : false;

		while (true) {
			//print the chess
			system("cls"); //clean the screen
			gobang.printBoard();

			//if it's player..
			if (isPlayer) {
				cout << "now it's your turn\n"
					<< "please input like \"1 a\"\n";
				if (!gobang.getIsAIFirstChess()) //print the ai information about what is put last turn
					cout << "AI last put at: " << gobang.getAILastStep().x + 1 << " " << char(gobang.getAILastStep().y + 'A') << endl;
				
				do {
					x = getIntValue([&](int value)->bool {return value > 0 && value <= BOARDSIZE; }); //[1.. 15]
					y = getCharValue([&](char value)->bool {return (value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z'); });
				} while (!gobang.playerTurn(currentChessType, x - 1, toupper(y)));//x=[0..14]

				//switch
				isPlayer = false;
			}
			else { // if it's computer..
				cout << "now it's AI's turn\n";
				gobang.aiTurn(currentChessType);

				//switch
				isPlayer = true;
			}

			//judge
			result = gobang.judgeVictory();
			if (result == BLACK || result == WHITE) {
				system("cls"); //clean the screen
				gobang.printBoard();
				cout << (result == BLACK ? "Black" : "White") << " is the winner\n";
				break;
			}
			else if (result == DRAW) {
				system("cls"); //clean the screen
				gobang.printBoard();
				cout << "draw!!\n";
				break;
			}

			//switch chessType
			currentChessType = currentChessType == BLACK ? WHITE : BLACK;
		}
		break;
	}
	default:
		break;
	}

	return 0;
}