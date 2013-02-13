#include "Maze.h"
#include "Block.h"
#include "MyControl.h"
#include <cairomm/context.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <sstream>

const Gdk::Color BLOCK_COLORS[BLOCK_NAMES_SIZE] = {
    Gdk::Color("white"),
    Gdk::Color("black"),
    Gdk::Color("blue"),
    Gdk::Color("red")
};
const char* BLOCK_NAMES[BLOCK_NAMES_SIZE] = {
    "Free",
    "Blocked",
    "Start",
    "Goal"
};

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
void Maze::createMaze(int i_rows, int i_columns) {

    // delete old data
    deleteMaze();

    // init
    m_rows = i_rows;
    m_columns = i_columns;

    // all blocks empty
    for (int i = 0; i < (m_rows * m_columns); i++) {
	int row = (int) i / m_columns;
	int column = (int) i % m_columns;
	m_blocks[i] = new Block(i, row, column, 0);
    }

    queue_draw();
}

/* delete old Maze
 */
void Maze::deleteMaze () {

    // clear search
    clearSearch();

    // remove all blocks
    int num = m_blocks.size();
    for (int i = 0; i < num; i++) {
	free(m_blocks[i]);
    }
    m_blocks.clear();
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

/* ############################## draw ###################################### */

/* draw
 */
bool Maze::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {

    Gtk::Allocation allocation = get_allocation();
    m_maze_width = allocation.get_width();
    m_maze_height = allocation.get_height() - SIZEOFLEGEND;

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
    m_block_width = m_maze_width / m_columns;
    m_block_height = m_maze_height / m_rows;

    // draw all blocks
    for (int i = 0; i < (int) m_blocks.size(); i++) {
	m_blocks[i]->drawme(cr, m_block_width, m_block_height);
    }

    // print some information
    stringstream ss;
    ss << "Maze";
    drawText(cr, ss.str().c_str());

    // print expanded search tree
    if (!isPaintable()) {
	cr->set_line_width(5.0);
	cr->set_source_rgb(0.8, 0.0, 0.0);
	drawTree(cr, m_stree);
    }

    // draw legend
    const char* msg = "Legend";
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(0, m_maze_height+LEGENDPADDING);
    drawText(cr, msg);

    // draw legend blocks
    for (int i = 0; i < (int) (sizeof(BLOCK_COLORS)/sizeof(Gdk::Color)); i++) {
	const int pos_x = LEGENDPADDING+i*LEGEND_BLOCK_SPACE;
	const int pos_y = m_maze_height+LEGENDPADDING+LEGEND_BLOCK_POSY;
	cr->set_source_rgb(
	    BLOCK_COLORS[i].get_red_p(),
	    BLOCK_COLORS[i].get_green_p(),
	    BLOCK_COLORS[i].get_blue_p()
	);
	cr->rectangle(pos_x, pos_y, LEGEND_BLOCK_WIDTH, LEGEND_BLOCK_HEIGHT);
	cr->fill();

	// label
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->move_to(pos_x + LEGEND_BLOCK_WIDTH + LEGENDPADDING, pos_y);
	drawText(cr, BLOCK_NAMES[i]);
    }

    // draw status
    if (isPaintable()) {
	msg = "Draw/load/edit maze";
    } else {
	msg = "Running...";
    }
    cr->move_to(m_maze_width/2+LEGENDPADDING, m_maze_height+LEGENDPADDING);
    drawText(cr, msg);

    return true;
}

/* draw tree
 * @param cr: cairo
 * @param t_element*: element to draw
 */
void Maze::drawTree(const Cairo::RefPtr<Cairo::Context>& cr, t_element* current) {

    // get position of current
    double pos_from_x = current->block->getX(m_block_width) + m_block_width/2;
    double pos_from_y = current->block->getY(m_block_height) + m_block_height/2;

    // draw lines
    for (int i = 0; i < (int) (sizeof(current->subs)/sizeof(t_element*)); i++) {
	if (current->subs[i] == NULL) break;
	double pos_to_x = current->subs[i]->block->getX(m_block_width) + m_block_width/2;
	double pos_to_y = current->subs[i]->block->getY(m_block_height) + m_block_height/2;
	cr->move_to(0,0);
	cr->move_to(pos_from_x, pos_from_y);
	cr->line_to(pos_to_x, pos_to_y);
	cr->stroke();
	drawTree(cr, current->subs[i]);
    }
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
	int row = (int) (pos_y / m_block_height);
	int column = (int) (pos_x / m_block_width);

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

/* ############################# search ##################################### */

/* init search tree
 */
bool Maze::initSearch() {
    m_stree = NULL;

    // get root and goal
    for (int i = 0; i < (m_rows * m_columns); i++) {
	if (m_blocks[i]->isRoot()) {
	    if (m_stree != NULL) return false;
	    m_stree = createNode(m_blocks[i]);
	}
    }

    // add to expand list
    m_sexpand = m_stree;

    return (m_sexpand == NULL) ? false : true;
}

/* clear search
 */
void Maze::clearSearch() {

    // empty m_stree
    removeTree(m_stree);

    // unset all expand information
    for (int i = 0; i < (int) m_blocks.size(); i++) {
	m_blocks[i]->setExpanded(false);
    }

    // set NULL
    m_sexpand = NULL;
    m_stree = NULL;
}

/* loop through tree
 */
void Maze::removeTree(t_element* current) {

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
t_element* Maze::createNode (Block* i_Block) {

    // get new node
    t_element* out = (t_element*) malloc(sizeof(t_element));
    if (out == NULL) {
	cout << "Malloc failed!\n";
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
void Maze::addToExpandList (t_element* newone) {
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
bool Maze::expandNode (t_element* current) {
    int id = current->block->getId();
    current->block->setExpanded(true);
    cout << "Expand node " << id << "\n";

    // get ids of neigbours
    int neighbours[] = {
	(id - m_columns), // above
	(id + m_columns), // below
	(id + 1), // right
	(id - 1) // left
    };

    // is right/left in same row?
    if (neighbours[2]/m_columns != id/m_columns) neighbours[2] = -1;
    if (neighbours[3]/m_columns != id/m_columns) neighbours[3] = -1;

    // add new elements to list
    for (int i = 0; i < 4; i++) {

	// get block
	Block* block = (
	    isBlock(neighbours[i]) &&
	    (current->root == NULL || current->root->block->getId() != neighbours[i])
	) ? m_blocks[neighbours[i]] : NULL;
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
bool Maze::isBlockExpanded (t_element* current, Block* block) {

    // found?
    if (current->block == block && current->block->isExpanded()) return true;

    // search sub elements
    for (int i = 0; i < (int) (sizeof(current->subs)/(sizeof(t_element*))); i++) {
	if (current->subs[i] != NULL && isBlockExpanded(current->subs[i], block))
	    return true;
    }

    return false;
}

/* run
 */
bool Maze::run (int) {
    cout << "Maze::run()\n";

    t_element* ccc = m_sexpand;
    while (ccc != NULL) {
	cerr << "in list: " << ccc->block->getId() << " - sum: " << ccc->sum << " - exp: " << (ccc->block->isExpanded() ? "true" : "false") << "\n";
	ccc = ccc->next;
    }

    // get next node to expand
    t_element* current = m_sexpand;
    while (current != NULL && current->block->isExpanded()) {
	current = current->next;
    }
    m_sexpand = current->next;

    // is node?
    if (current == NULL) {
	cout << "ERROR: empty list or goal unreachable!\n";
	m_Contr->on_menu_stop();
	return false;
    }

    // update next
    current->next = NULL;

    // reached goal?
    if (current->block->isGoal()) {
	cout << "Reached goal!\n";
	m_Contr->on_menu_pause();
	return false;
    }

    // expand
    expandNode(current);

    // draw
    queue_draw();

    return true;
}

/* ########################## save/load ##################################### */

/* save file
 */
bool Maze::saveMaze(const string* path) {
    if (path == NULL) return false;
    char c = '|';

    // write
    ofstream myfile;
    myfile.open(path->c_str());
    myfile << m_rows << c << m_columns << c;

    // add all blocks
    for (int i = 0; i < (int) m_blocks.size(); i++) {
	myfile << m_blocks[i]->getType() << c;
    }

    myfile.close();
    return true;
}

/* open file
 */
bool Maze::loadMaze(const string* path) {
    if (path == NULL) return false;
    char c = '|';
    char d;
    stringstream ss("");

    // open for reading
    ifstream myfile(path->c_str());

    // get m_rows
    d = 'a';
    while (d != c) {
	if (d != 'a') ss << d;
	myfile >> d;
    }
    ss >> m_rows;

    // get m_columns
    d = 'a';
    while (d != c) {
	if (d != 'a') ss << d;
	myfile >> d;
    }
    ss >> m_columns;

    // create new maze
    createMaze(m_rows, m_columns);

    int i = 0;
    d = 'a';
    myfile >> d >> c;
    while (!myfile.eof()) {
	m_blocks[i]->setType(d - '0');
	i++;
	myfile >> d >> c;
    }

    myfile.close();

    queue_draw();

    return true;
}
