#ifndef _SEARCHBREADTHFIRST_H
#define _SEARCHBREADTFIRST_H

#include "Search.h"

class Maze;

class SearchBreadthFirst : public Search {

    public:
	SearchBreadthFirst(Maze* i_Maze);

	bool init(int i_rows, int i_columns);
	int run();
	void addToExpandList(t_element* newone);
	bool expandNode(t_element*);
};

#endif // _SEARCHBREADTHFIRST_H
