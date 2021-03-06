#include "SearchA.h"
#include "Maze.h"
#include "Block.h"
#include <iostream>

using namespace std;

/* constructor
 * @param Maze*: Maze object
 */
SearchA::SearchA (Maze* i_Maze)
    : Search(i_Maze)
{
    m_stree = NULL;
    m_sexpand = NULL;
}

/* init search
 * @param int: number of rows
 * @param int: number of columns
 */
bool SearchA::init (int i_rows, int i_columns) {
    m_stree = NULL;

    // get root and goal
    for (int i = 0; i < (i_rows * i_columns); i++) {
	Block* block = m_Maze->getBlock(i);
	if (block == NULL) continue;
	if (block->isRoot()) {
	    if (m_stree != NULL) return false;
	    m_stree = createNode(block);
	}
	if (block->isGoal()) {
	    m_goal = block;
	}
    }

    // add to expand list
    m_sexpand = m_stree;

    return (m_sexpand == NULL) ? false : true;
}

/* run next step of search
 */
int SearchA::run() {

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

/* add to list
 * @param t_element*: element to be added
 */
void SearchA::addToExpandList (t_element* newone) {
    t_element* before = NULL;
    t_element* current = m_sexpand;

    // forward to right place
    while (current != NULL && current->sum2 <= newone->sum2) {
	before = current;
	current = current->next;
    }

    // add to list
    if (before == NULL) {
	// empty list
	m_sexpand = newone;
    } else {
	newone->next = current;
	before->next = newone;
    }
}

/* expand node
 * @param t_element*: node to be expanded
 */
bool SearchA::expandNode (t_element* current) {
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
	elem->sum2 = elem->sum + getDistance(elem);

	// add to list
	addToExpandList(elem);
    }

    return true;
}

/* get distance to goal
 * @param t_element*: element to measure from
 */
double SearchA::getDistance(t_element* i_from) {
    double diff_x = abs(i_from->block->getColumn() - m_goal->getColumn());
    double diff_y = abs(i_from->block->getRow() - m_goal->getRow());
    return sqrt(diff_x*diff_x + diff_y*diff_y);
}
