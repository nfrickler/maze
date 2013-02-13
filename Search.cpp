#include "Search.h"
#include "Maze.h"
#include "Block.h"
#include <iostream>

using namespace std;

/* constructor
 */
Search::Search (Maze* i_Maze)
    : m_Maze(i_Maze)
{
    m_stree = NULL;
    m_sexpand = NULL;
}

bool Search::init (int i_rows, int i_columns) {
    m_stree = NULL;

    // get root and goal
    for (int i = 0; i < (i_rows * i_columns); i++) {
	Block* block = m_Maze->getBlock(i);
	if (block == NULL) continue;
	if (block->isRoot()) {
	    if (m_stree != NULL) return false;
	    m_stree = createNode(block);
	}
    }

    // add to expand list
    m_sexpand = m_stree;

    return (m_sexpand == NULL) ? false : true;
}

void Search::clear () {
    
    // empty m_stree
    removeTree(m_stree);

    // unset all expand information
    for (int i = 0; i < m_Maze->getMaxId(); i++) {
	m_Maze->getBlock(i)->setExpanded(false);
    }

    // set NULL
    m_sexpand = NULL;
    m_stree = NULL;
}

/* loop through tree
 */
void Search::removeTree(t_element* current) {

    // is element?
    if (current == NULL) return;

    // remove subs
    for (int i = 0; i < (int) (sizeof(current->subs)/(sizeof(t_element*))); i++) {
	if (current->subs[i] != NULL) removeTree(current->subs[i]);

    }

    // remove this element
    free(current);
}

/* create new node
 */
t_element* Search::createNode (Block* i_Block) {

    // get new node
    t_element* out = (t_element*) malloc(sizeof(t_element));
    if (out == NULL) {
	cerr << "Malloc failed!\n";
	return NULL;
    }

    // fill
    out->block = i_Block;
    out->sum = 0;
    out->root = NULL;

    for (int i = 0; i < (int) (sizeof(out->subs)/(sizeof(t_element*))); i++) {
	out->subs[i] = NULL;
    }
    out->next = NULL;

    return out;
}

/* add to list
 */
void Search::addToExpandList (t_element* newone) {
    t_element* current = m_sexpand;
    int pos = 0;

    // forward to right place
    while (current != NULL && current->next != NULL && current->next->sum <= newone->sum) {
	current = current->next;
	pos++;
    }

    // add to list
    if (current == NULL) {
	// empty list
	m_sexpand= newone;
    } else {
	newone->next = current->next;
	current->next = newone;
    }
}

/* expand node
 */
bool Search::expandNode (t_element* current) {
    int id = current->block->getId();
    int colnum = m_Maze->getColumnNum();
    current->block->setExpanded(true);
    cout << "Expand node " << id << "\n";

    // get ids of neigbours
    int neighbours[] = {
	(id - colnum), // above
	(id + colnum), // below
	(id + 1), // right
	(id - 1) // left
    };

    // is right/left in same row?
    if (neighbours[2]/colnum != id/colnum) neighbours[2] = -1;
    if (neighbours[3]/colnum != id/colnum) neighbours[3] = -1;

    // add new elements to list
    for (int i = 0; i < 4; i++) {

	// get block
	Block* block = (
	    m_Maze->isBlock(neighbours[i]) &&
	    (current->root == NULL || current->root->block->getId() != neighbours[i])
	) ? m_Maze->getBlock(neighbours[i]) : NULL;
	if (block == NULL) continue;

	// is solid block?
	if (block->isSolid()) continue;

	// create new element
	t_element* elem = createNode(block);
	if (elem == NULL) {
	    cerr << "ERROR: Failed to create node!\n";
	    return false;
	}

	// add this element as sub to current
	for (int i = 0; i < (int) (sizeof(current->subs)/(sizeof(t_element*))); i++) {
	    if (current->subs[i] == NULL) {
		current->subs[i] = elem;
		break;
	    }
	}

	// set root and new sum
	elem->root = current;
	elem->sum = current->sum + 1;

	// add to list
	addToExpandList(elem);
    }

    return true;
}

/* is block in m_stree
 */
bool Search::isBlockExpanded (t_element* current, Block* block) {

    // found?
    if (current->block == block && current->block->isExpanded()) return true;

    // search sub elements
    for (int i = 0; i < (int) (sizeof(current->subs)/(sizeof(t_element*))); i++) {
	if (current->subs[i] != NULL && isBlockExpanded(current->subs[i], block))
	    return true;
    }

    return false;
}

int Search::run () {
    
    t_element* ccc = m_sexpand;
    while (ccc != NULL) {
	ccc = ccc->next;
    }

    // get next node to expand
    t_element* current = m_sexpand;
    while (current != NULL && current->block->isExpanded()) {
	current = current->next;
    }

    // is node?
    if (current == NULL) return 1;

    // update list
    m_sexpand = current->next;
    current->next = NULL;

    // reached goal?
    if (current->block->isGoal()) return 2;

    // expand
    expandNode(current);

    return 0;
}

/* draw tree
 * @param cr: cairo
 * @param t_element*: element to draw
 */
void Search::drawTree(const Cairo::RefPtr<Cairo::Context>& cr, t_element* current, int i_block_width, int i_block_height) {

    // init with m_sexpand
    if (current == NULL) current = m_stree;

    // get position of current
    double pos_from_x = current->block->getX(i_block_width) + i_block_width/2;
    double pos_from_y = current->block->getY(i_block_height) + i_block_height/2;

    // draw lines
    for (int i = 0; i < (int) (sizeof(current->subs)/sizeof(t_element*)); i++) {
	if (current->subs[i] == NULL) break;
	double pos_to_x = current->subs[i]->block->getX(i_block_width) + i_block_width/2;
	double pos_to_y = current->subs[i]->block->getY(i_block_height) + i_block_height/2;
	cr->move_to(0,0);
	cr->move_to(pos_from_x, pos_from_y);
	cr->line_to(pos_to_x, pos_to_y);
	cr->stroke();
	drawTree(cr, current->subs[i], i_block_width, i_block_height);
    }
}
