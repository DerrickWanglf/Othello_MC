#include"MCNode.h"
#include <stdio.h>
#include"iomanip"
#include "constant.h"



MCNode::MCNode(Board *othello){
	this->othello = othello;
	children = vector<MCNode*>(othello->playset.size(), (MCNode*)0);
	parent = 0;
	UCT_v  = 0;
    visit_times = 0;
    blackCount = whiteCount = 0;
    time_li = 2;
}

int MCNode::Policy() {
	const double weight = 2;
	double uctval = -2;
	double treeval;
	int ucb = -1;

	for (int i = 0; i < children.size(); i++) {
		MCNode * subnode = children[i];
		if (subnode == NULL) return i;
		if (subnode->UCT_v == 0) return i;

		double temp = sqrt(log((double)UCT_v / subnode->UCT_v));
        treeval = (double)subnode->visit_times / subnode->UCT_v + weight * temp; // fomula of UCT
		// asign
        if (treeval > uctval) {
			uctval = treeval;
			ucb = i;
		}
	}
	return ucb;
}

int MCNode::Search() {
	MCNode *pre_node = 0;
	MCNode *cur_node = this;
	int index;
	// recursively get the child to be expanded
	while (cur_node) {
		pre_node = cur_node;
		index = pre_node->Policy();
		if (index == -1) {
			return 1;
		}
		else {
			cur_node = pre_node->children[index];
		}
	}

	// expand the node
	Board * cur_board = new Board(*(pre_node->othello));
	cur_board->play(cur_board->playset[index]);
	cur_node = new MCNode(cur_board);
	cur_node->parent = pre_node;
	pre_node->children[index] = cur_node;

	// simulation
	int node_si = pre_node->DefaultPolicy();

	if (node_si == othello->turn){
		cur_node->BP(1);
	} else if(node_si == -(othello->turn)) {
		cur_node->BP(0);
	} else {
		cur_node->BP(0.5);
	}
	return 0;
}

MCNode* MCNode::AIPlay(float timelim) {

	//get the time limit
	const double timelimit = getTimelimit(timelim);

	std::cout<<endl<<"searching..............."<<endl;

	time_t s_time, t_time;
	double searchtime = 0 ;
	int i = 0;
	int over;

	while(1){
		s_time = clock();
		over = Search();
		i++;
		t_time = clock();
		if (over) { std::cout << "Search End" << std::endl; break; }
		double diff = double( t_time - s_time)/1000000;
		searchtime += diff;
		if (searchtime > timelimit) break;

	}

	if(searchtime > timelimit) searchtime = timelimit;
	std::cout << "searchtime: " << fixed<<setprecision(6)<<searchtime << std::endl;
	
	// choose the best child
	int n = -1;
	double maxucb = -2;
	double tucb;
	for (int i = 0; i < children.size(); i++) {
		if (!children[i]) continue;
		tucb = (double)children[i]->visit_times / children[i]->UCT_v;
		if (tucb > maxucb){
			maxucb = tucb;
			n = i;
		}
	}
	if (n < 0) {
		std::cout << "Search Not Enough!" << std::endl;
		return this;
	}

	std::cout << "Play " << Board::to2dx(othello->playset[n].pos) << " " << Board::to2dy(othello->playset[n].pos) << " "
		<< "\tWith " << i << " searches" << std::endl;

	// play out
	return Play(n);
}


int MCNode::DefaultPolicy() {
	// randomly roll out
	Board board(*othello);
	while (board.boardstate == Board::PLAYING) {
		board.randomplay();
	}
	return board.boardstate;
}


void MCNode::BP(double val) {
	for (MCNode *tnode = this; tnode; tnode = tnode->parent) {
		tnode->UCT_v++; tnode->visit_times += val;
	}
}

MCNode* MCNode::Play(int n) {
	MCNode *sub_node = children[n];
	if (!sub_node) {
		Board *tothello = new Board(*othello);
		tothello->play(tothello->playset[n]);
		sub_node = new MCNode(tothello);
	}

	sub_node->parent = 0;
	this->children[n] = 0;
	freetree();
	return sub_node;
}

MCNode* MCNode::Play(int x, int y) {
	int n = -1;
	for (int i = 0; i < othello->playset.size(); i++) {
		if (Board::to1d(x, y) == othello->playset[i].pos) { n = i; break; }
	}
	if (n >= 0) return Play(n);
	else {
		std::cout << "Invalid play" << std::endl;
		return this;
	}
}

MCNode::~MCNode() {
	delete othello;
}

void MCNode::freetree() {
	for (int i = 0; i < children.size(); i++) {
		if(children[i]) children[i]->freetree();
	}
	delete this;
}


