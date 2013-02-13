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

/* ############# to be implemented in childs ########### */

/* init search
 * @param int: number of rows
 * @param int: number of columns
 */
bool Search::init (int i_rows, int i_columns) {
    return false;
}

/* run next step of search
 */
int Search::run () {
    return 3;
}

/* add to list
 */
void Search::addToExpandList (t_element*) {
    return;
}

/* expand node
 */
bool Search::expandNode (t_element*) {
    return false;
}
