#pragma once
#include<iostream>
#include<math.h>
#include<time.h>
#include "constant.h"
#include"board.h"

// a Monte Carlo Search Tree Node
class MCNode {
public:
	MCNode(Board *othello);

	// game state
	Board *othello;
	// number of chesses on the board
    int blackCount;
    int whiteCount;
	// Monte Carlo Value for the calcutation of UCT
	int vn;
	int time_li;
	// action
	double an;
	
	vector<MCNode*> children;
	MCNode *parent;

    MCNode* SearchAndPlay(int n);
	MCNode* Play(int n);
	MCNode* Play(int x, int y);
	void BackPropagation(double val);
	void freetree();
	//int fullexpanded;
	~MCNode();

	//FOT TEST, print out the tree, can be just a number of levels
	void showtree(int level, int tatrgetlevel);
	void showtree(int level);

	double getTimelimit(int n){
        double base = n/2;
        double childrenfactor;
        double processfactor;

        int csize = children.size();
        if(csize == 1) return 0;
        if (csize > 10) csize = 10;
        childrenfactor = csize / 2.0;

        int count = blackCount + whiteCount;
        if (count < 32) processfactor = 0;
        else {
            processfactor = (32.0 - count) / 32.0 * 15.0;
        }

        double res = base + childrenfactor + processfactor;
        if(res > 55) res = 55;
        if(res > n) res = n;
        return res;
    }

private:
	// get the search time bound


	int TreePolicy();
	int DefaultPolicy();
	int SearchOnce();
};
