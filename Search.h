#ifndef _SEARCH_H
#define _SEARCH_H

#include <cairomm/context.h>

class Maze;
class Block;

typedef struct s_element {
    Block* block;
    int sum;
    double sum2;
    struct s_element* root;
    struct s_element* subs[4];
    struct s_element* next;
} t_element;

class Search {

    public:
	Search(Maze* i_Maze);

	// search
	void clear();
	t_element* createNode(Block* i_Block);
	void removeTree(t_element* current);

	// status
	bool isBlockExpanded(t_element* current, Block* block);

	// drawing
	void drawTree(const Cairo::RefPtr<Cairo::Context>& cr, t_element* current, int i_block_width, int i_block_height);

	// to be implemented in childs
	virtual bool init(int i_rows, int i_columns);
	virtual int run();
	virtual void addToExpandList(t_element* newone);
	virtual bool expandNode(t_element*);

    protected:
	Maze* m_Maze;

	// lists and trees
	t_element* m_stree;
	t_element* m_sexpand;
};

#endif // _SEARCH_H
