#pragma once
#include<iostream>
#include<math.h>
#include<time.h>
#include "constant.h"
#include"board.h"


class MCNode {
public:
	MCNode(Board *othello);

	Board *othello; // current board

    int blackCount;//number of black
    int whiteCount;//number of white

	int UCT_v;
	int time_li;

	double visit_times;
	
	vector<MCNode*> children;
	MCNode *parent;

    MCNode* AIPlay(float n);
	MCNode* Play(int n);
	MCNode* Play(int x, int y);
	void BP(double val);

	void freetree();
	//int fullexpanded;
	~MCNode();

	double getTimelimit(float n){
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
        return n;
    }
    int Policy();
    int DefaultPolicy();
    int Search();

};
