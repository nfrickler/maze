#include "Block.h"
#include <iostream>

using namespace std;

/* construct
 * @param int: id
 * @param int: row
 * @param int: column
 * @param int: type
 */
Block::Block(int i_id, int i_row, int i_column, int i_type)
    : m_id(i_id), m_row(i_row), m_column(i_column), m_type(i_type)
{
    m_isexpanded = false;

}

/* get id of block
 */
int Block::getId() {
    return m_id;
}

/* draw this block
 */
bool Block::drawme(Cairo::RefPtr<Cairo::Context> cr, double i_width, double i_height) {

    // get position of block
    double pos_x = getX(i_width);
    double pos_y = getY(i_height);

    // draw
    cr->move_to(0,0);
    switch (m_type) {
	case 1:
	    cr->set_source_rgb(0.0, 0.0, 0.0);
	    break;
	case 2:
	    cr->set_source_rgb(0.0, 0.0, 1.0);
	    break;
	case 3:
	    cr->set_source_rgb(1.0, 0.0, 0.0);
	    break;
	default:
	    cr->set_source_rgb(1.0, 1.0, 1.0);
	    break;
    }
    cr->rectangle(pos_x, pos_y, i_width, i_height);
    cr->fill();

    // draw outline
    cr->set_line_width(0.2);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(pos_x,pos_y);
    cr->line_to(pos_x+i_width-1,pos_y+0);
    cr->line_to(pos_x+i_width-1,pos_y+i_height-1);
    cr->line_to(pos_x+0,pos_y+i_height-1);
    cr->line_to(pos_x+0,pos_y+0);
    cr->stroke();

    return true;
}

/* setter and getter */
void Block::setType(int i_type) { m_type = i_type; }
int Block::getType() { return m_type; }
bool Block::isRoot() { return (getType() == 2) ? true : false; }
bool Block::isGoal() { return (getType() == 3) ? true : false; }
bool Block::isSolid() { return (getType() == 1) ? true : false; }
double Block::getX(int i_width) {
    return m_column * i_width;
}
double Block::getY(int i_height) {
    return m_row * i_height;
}
void Block::setExpanded(bool i_exp) { m_isexpanded = i_exp; }
bool Block::isExpanded() { return m_isexpanded; }
