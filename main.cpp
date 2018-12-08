#include<iostream>
#include<stdlib.h>
#include<time.h>
#include "constant.h"
#include"board.h"
#include"MCNode.h"

using namespace std;

int main() {
	char c;
	int a, b;
	Board* othello = new Board();
	othello->showboard();
	MCNode *mtree=  new MCNode(othello);
	const int COMPUTER = WHITE;
	srand(time(0));
	while (mtree->othello->boardstate == Board::PLAYING) {
		if (mtree->othello->turn == COMPUTER) {
			mtree = mtree->SearchAndPlay();
		}
		else {

			int moveNum = -1;
			string in;
			if (!mtree->othello->playset.empty()) {
				for (int i = 0; i < mtree->othello->playset.size(); i++) {
					cout << i << ": [" << Board::to2dx(mtree->othello->playset[i].pos) << "," << Board::to2dy(mtree->othello->playset[i].pos) << "]" << endl;
				}

				while (!(moveNum < mtree->othello->playset.size() && moveNum >= 0)) {
					cout << "Your Move: " << endl;
					cin >> in;
					if (isdigit(in.c_str()[0])) {
						moveNum = stoi(in);
					}
				}
				int pp = mtree->othello->playset[moveNum].pos;
				mtree = mtree->Play(Board::to2dx(pp),Board::to2dy(pp));

			}
/*
				std::cin >> a;
			if (a == -1) {
				srand(time(0));
				mtree = mtree->Play(rand() % mtree->othello->playset.size());
			}
			else {
				std::cin >> b;
				mtree = mtree->Play(a, b);
			}*/
		}
		mtree->othello->showboard();
	}
	if (mtree->othello->boardstate == WHITE) {
		std::cout << "WINNER IS WHITE!" << std::endl;
	}
	else if (mtree->othello->boardstate == BLACK) {
		std::cout << "WINNER IS BLACK!" << std::endl;
	}else {
		std::cout << "DRAW!" << std::endl;
	}
	std::cin >> c;

}
