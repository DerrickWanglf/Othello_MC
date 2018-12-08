#include"MCNode.h"
#include "constant.h"


//initial node, with k children, k is the number of next move
MCNode::MCNode(Board *othello){
	this->othello = othello;
	// determine the number of children according to the playset of the give Othello
	children = vector<MCNode*>(othello->playset.size(), (MCNode*)0);
	parent = 0;
	vn = an = 0;
    blackCount = whiteCount = 0;
}

int MCNode::TreePolicy() {
	const double r = 2;
	double ucbval = -2, tval;
	// if there are no children, -1 of ucbi will be returned
	int ucbi = -1;
	for (int i = 0; i < children.size(); i++) {
		MCNode *tnode = children[i];
		if (!tnode) return i;
		if (!tnode->vn) return i;
		// use the fomula of UCT
		tval = (double)tnode->an / tnode->vn + r * sqrt(log((double)vn / tnode->vn));
		if (tval > ucbval) { ucbval = tval; ucbi = i; }
	}
	return ucbi;
}

int MCNode::SearchOnce() {
	MCNode *tnode = 0;
	MCNode *nnode = this;
	int n;
	// recursively get the child to be expanded
	while (nnode) {
		tnode = nnode;
		n = tnode->TreePolicy();
		if (n == -1) {
			return 1;
		}
		else {
			nnode = tnode->children[n];
		}
	}

	// expand the node
	Board *tothello = new Board(*(tnode->othello));
	tothello->play(tothello->playset[n]);
	nnode = new MCNode(tothello);
	nnode->parent = tnode;
	tnode->children[n] = nnode;

	// simulation and backPropagaion
	int simuv = tnode->DefaultPolicy();
	if (simuv == othello->turn) {
		nnode->BackPropagation(1);
	} else if(simuv == -(othello->turn)) {
		nnode->BackPropagation(0);
	} else {
		nnode->BackPropagation(0.5);
	}
	return 0;
}

MCNode* MCNode::SearchAndPlay() {
	//get the time limit
	const double timelimit = getTimelimit();

	std::cout<<endl<<"searching..............."<<endl;
	std::cout << "searchtime: " << timelimit << std::endl;
	time_t s_time, t_time;
	time(&s_time);

	int i = 0;
	int searchend;
	// MCTSing until search ends or reach the time limit
	while(1){
		searchend = SearchOnce();
		if (searchend) { std::cout << "Search End" << std::endl; break; }
		i++;
		time(&t_time);
		if (difftime(t_time, s_time) > timelimit) break;
	}

	// choose the best child
	int n = -1;
	double maxucb = -2;
	double tucb;
	for (int i = 0; i < children.size(); i++) {
		if (!children[i]) continue;
		tucb = (double)children[i]->an / children[i]->vn;
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
	Board tothello(*othello);
	while (tothello.boardstate == Board::PLAYING) {
		tothello.randomplay();
	}
	return tothello.boardstate;
}


void MCNode::BackPropagation(double val) {
	for (MCNode *tnode = this; tnode; tnode = tnode->parent) {
		tnode->vn++; tnode->an += val;
	}
}

MCNode* MCNode::Play(int n) {
	MCNode *nnode = children[n];
	if (!nnode) {
		Board *tothello = new Board(*othello);
		tothello->play(tothello->playset[n]);
		nnode = new MCNode(tothello);
	}
	// separate the child and the root
	// then free the tree of the root
	nnode->parent = 0;
	this->children[n] = 0;
	freetree();
	return nnode;
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


void MCNode::showtree(int level) {
	for (int i = 0; i < level; i++) {
		std::cout << "  ";
	}
	std::cout << "|| " << an << " / " << vn << std::endl;
	for (int i = 0; i < children.size(); i++) {
		if(children[i]) children[i]->showtree(level + 1);
	}
}

void MCNode::showtree(int level, int targetlevel) {

	for (int i = 0; i < level; i++) {
		std::cout << "  ";
	}
	std::cout << "|| " << an << " / " << vn << std::endl;
	if (level >= targetlevel) return;
	for (int i = 0; i < children.size(); i++) {
		for (int i = 0; i < level + 1; i++) {
			std::cout << "  ";
		}
		std::cout << "x= " <<  Board::to2dx(othello->playset[i].pos)<< " y= " << Board::to2dy(othello->playset[i].pos) << std::endl;
		if (children[i]) children[i]->showtree(level + 1, targetlevel);
	}
}
