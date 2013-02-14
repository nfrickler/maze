#ifndef _SEARCHDEPTHFIRST_H
#define _SEARCHDEPTHFIRST_H

#include "Search.h"

class Maze;

class SearchDepthFirst : public Search {

    public:
	SearchDepthFirst(Maze* i_Maze);

	bool init(int i_rows, int i_columns);
	int run();
	void addToExpandList(t_element* newone);
	bool expandNode(t_element*);
};

#endif // _SEARCHDEPTHFIRST_H
