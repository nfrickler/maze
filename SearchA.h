#ifndef _SEARCHA_H
#define _SEARCHA_H

#include "Search.h"

class Maze;
class Block;

class SearchA : public Search {

    public:
	SearchA(Maze* i_Maze);

	bool init(int i_rows, int i_columns);
	int run();
	void addToExpandList(t_element* newone);
	bool expandNode(t_element*);

    protected:
	Block* m_goal;
	double getDistance(t_element* i_from);
};

#endif // _SEARCHA_H
