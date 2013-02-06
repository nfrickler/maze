#include "Maze.h"
#include "MyControl.h"
#include <cairomm/context.h>
#include <iostream>

using namespace std;

/* constructor
 */
Maze::Maze (MyControl* i_Contr)
    : m_Contr(i_Contr)
{
    this->add_events(Gdk::BUTTON_PRESS_MASK);
    this->signal_event().connect(
	sigc::mem_fun(*this, &Maze::on_event_happend)
    );
    m_is_paintable = true;
}

/* create empty Maze
 * @param int: width of new Maze
 * @param int: height of new Maze
 */
void Maze::create (int i_rows, int i_columns) {

    // init
    m_rows = i_rows;
    m_columns = i_columns;

    // all blocks empty
    for (int i = 0; i < (m_rows * m_columns); i++) {
	int row = (int) i / m_columns;
	int column = (int) i % m_columns;
	cout << "New Block (" << row << ", " << column << ")\n";
	m_blocks[i] = new Block(i, row, column, 0);
    }
}

/* draw
 */
bool Maze::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {

    Gtk::Allocation allocation = get_allocation();
    m_maze_width = allocation.get_width();
    m_maze_height = allocation.get_height();

    cr->set_line_width(5.0);

    // mark end of field
    cr->set_source_rgb(0.8, 0.0, 0.0);
    cr->move_to(0,0);
    cr->line_to(m_maze_width,0);
    cr->line_to(m_maze_width,m_maze_height);
    cr->line_to(0,m_maze_height);
    cr->line_to(0,0);
    cr->stroke();

    // get size of blocks
    double block_width = m_maze_width / m_rows;
    double block_height = m_maze_height / m_columns;

    // draw all blocks
    for (int i = 0; i < (m_rows * m_columns); i++) {
	m_blocks[i]->drawme(cr, block_width, block_height);
    }

    // print some information
    cr->move_to(0,0);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    stringstream ss;
    ss << "Maze";
    drawText(cr, ss.str().c_str());

    // print all lines
    cr->set_line_width(5.0);
    cr->set_source_rgb(1.0, 1.0, 0.0);
    if (!isPaintable()) {
	for (int k = 0; k < (int) m_lines.size(); k++) {

	    // get positions
	    double pos_from_x = m_lines[k][0]->block->getX(block_width) + block_width/2;
	    double pos_from_y = m_lines[k][0]->block->getY(block_height) + block_height/2;
	    double pos_to_x = m_lines[k][1]->block->getX(block_width) + block_width/2;
	    double pos_to_y = m_lines[k][1]->block->getY(block_height) + block_height/2;

	    // draw line
	    cr->move_to(0,0);
	    cr->set_line_width(5.0);
	    cr->set_source_rgb(0.8, 0.0, 0.0);
	    cr->move_to(pos_from_x, pos_from_y);
	    cr->line_to(pos_to_x, pos_to_y);
	    cr->stroke();
	}
    }

    return true;
}

/* draw text
 * @param cr: cairo
 * @param const char[]: text to draw
 */
void Maze::drawText(const Cairo::RefPtr<Cairo::Context>& cr, const char text[])
{
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
    Pango::FontDescription font;

    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_BOLD);

    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
    Glib::RefPtr<Pango::Layout> layout = create_pango_layout(text);

    layout->set_font_description(font);

    // show
    layout->show_in_cairo_context(cr);
}

/* handle events
 */
bool Maze::on_event_happend(GdkEvent *event) {
    if (event->type == GDK_BUTTON_PRESS) {
	if (!isPaintable()) return false;
	double pos_x = event->button.x;
	double pos_y = event->button.y;
	int button = event->button.button;

	cout << "clicked: X= " << pos_x << " Y= " << pos_y << std::endl;

	// get position of Block
	int row = (int) (pos_x / ((int) (m_maze_width/m_columns)));
	int column = (int) (pos_y / ((int) (m_maze_height/m_rows)));

	// get id of block
	int i = (row) * m_rows + column;

	// is block?
	if (!isBlock(i)) return false;

	// what type of click?
	int newtype = 0;
	if (button == 1) {
	    if (m_blocks[i]->getType() == 0) {
		newtype = 1;
	    } else {
		newtype = 0;
	    }
	} else {
	    switch (m_blocks[i]->getType()) {
		case 2:
		    newtype = 3;
		    break;
		case 3:
		    newtype = 0;
		    break;
		default:
		    newtype = 2;
		    break;
	    }
	}
	m_blocks[i]->setType(newtype);

	// redraw
	queue_draw();
    }
    return false;
}

/* is Block with this id?
 * @param int: id
 */
bool Maze::isBlock (int i_id) {
    return (m_blocks.find(i_id) != m_blocks.end()) ? true : false;
}

/* getter and setter */
bool Maze::isPaintable() { return m_is_paintable; }
void Maze::setPaintable(bool i_bool) { m_is_paintable = i_bool; }

/* init search tree
 */
bool Maze::initSearch() {
    m_list = NULL;

    // get root and goal
    for (int i = 0; i < (m_rows * m_columns); i++) {
	if (m_blocks[i]->isRoot()) {
	    if (m_list != NULL) return false;
	    m_list = createNode(m_blocks[i]);
	}
    }

    return true;
}

/* create new node
 */
t_element* Maze::createNode (Block* i_Block) {

    // get new node
    t_element* out = (t_element*) malloc(sizeof(t_element));
    if (out == NULL) {
	cout << "Malloc failed!\n";
	return NULL;
    }

    // fill
    out->block = i_Block;
    out->root = 0;
    out->next = NULL;
    out->sum = 0;

    return out;
}

/* add to list
 */
void Maze::addToList (t_element* newone) {
    cout << "Add to list " << newone->block->getId() << "\n";
    t_element* current = m_list;
    int pos = 0;
    cout << "Add with sum=" << newone->sum << "\n";

    // forward to right place
    while (current != NULL && current->next != NULL && current->next->sum <= newone->sum) {
	cout << " - sum=" << current->sum << "\n";
	current = current->next;
	pos++;
    }

    cout << "Add element in list at pos = " << pos << "\n";

    // add to list
    if (current == NULL) {
	// empty list
	m_list = newone;
    } else {
	newone->next = current->next;
	current->next = newone;
    }

    current = m_list;
    while (current != NULL) {
	cout << "List: " << current->sum << "\n";
	current = current->next;
    }
}

/* expand node
 */
bool Maze::expandNode (t_element* current) {
    int id = current->block->getId();
    m_moved_from = current;
    cout << "Expand node " << id << "\n";

    // get ids of neigbours
    int neighbours[] = {
	(id - m_columns), // above
	(id + m_columns), // below
	(id + 1), // right
	(id - 1) // left
    };

    // add new elements to list
    for (int i = 0; i < 4; i++) {

	// get block
	Block* block = (
	    isBlock(neighbours[i]) && current->root != neighbours[i]
	) ? m_blocks[neighbours[i]] : NULL;
	if (block == NULL) continue;

	// is solid block?
	if (block->isSolid()) continue;

	// get element
	t_element* elem = createNode(block);
	m_moved_to = elem;

	// set root and new sum
	elem->root = id;
	elem->sum = current->sum + 1;

	// add to list
	addToList(elem);

	// add to lines
	int linessize = m_lines.size();
	m_lines[linessize][0] = current;
	m_lines[linessize][1] = elem;
    }

    return true;
}

/* run
 */
bool Maze::run (int) {
    cout << "Maze::run()\n";

    // get next node to expand
    t_element* next = m_list;
    if (next == NULL) {
	cout << "ERROR: empty list or goal unreachable!\n";
	m_Contr->stopMaze();
	return false;
    }
    m_list = next->next;

    // reached goal?
    if (next->block->isGoal()) {
	cout << "Reached goal!\n";
	m_Contr->stopMaze();
	return false;
    }

    // expand
    expandNode(next);

    // draw
    queue_draw();

    return true;
}
