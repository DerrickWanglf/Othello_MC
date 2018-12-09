#include<iostream>
#include<stdlib.h>
#include<time.h>
#include"board.h"
#include"MCNode.h"
#include "constant.h"

using namespace std;


int main(int argc, const char *argv[]) {
    int computer = WHITE;
    float timelimit;

	Board* othello = new Board();
	othello->showboard();
	MCNode *mtree=  new MCNode(othello);

	cout<<"PLEASE choose computer turn, Black 1, White -1:"<<endl;
	while(1){
		cin>>computer;
		if(computer == 1 || computer == -1)
			break;
		else
			cout<<"Wrong type"<<endl;
	}
    const int COMPUTER = computer;
	cout<<"PLEASE enter the base timelimit perstep:"<<endl;
	cin>>timelimit;

	while (mtree->othello->boardstate == Board::PLAYING) {
		if (mtree->othello->turn == COMPUTER) {
			mtree = mtree->AIPlay(timelimit);
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

	return  0;
}
