#ifndef _SEARCH_H
#define _SEARCH_H

#include <cairomm/context.h>

class Maze;
class Block;

typedef struct s_element {
    Block* block;
    int sum;
    struct s_element* root;
    struct s_element* subs[4];
    struct s_element* next;
} t_element;

class Search {

    public:
	Search(Maze* i_Maze);

	// search
	bool init(int i_rows, int i_columns);
	void clear();
	t_element* createNode(Block* i_Block);
	bool expandNode(t_element*);
	void addToExpandList(t_element* newone);
	int run();
	void removeTree(t_element* current);

	// status
	bool isBlockExpanded(t_element* current, Block* block);

	void drawTree(const Cairo::RefPtr<Cairo::Context>& cr, t_element* current, int i_block_width, int i_block_height);

    protected:
	Maze* m_Maze;

	// searching
	t_element* m_stree;
	t_element* m_sexpand;


};

#endif // _SEARCH_H
