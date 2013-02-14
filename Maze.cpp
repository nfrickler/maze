#include "Maze.h"
#include "Block.h"
#include "MyControl.h"
#include <cairomm/context.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include "Search.h"
#include "SearchBreadthFirst.h"
#include "SearchDepthFirst.h"
#include "SearchA.h"

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
    // init
    m_is_paintable = true;
    m_msg = "";

    // init Search objects
    m_searchtype = 0;
    m_Search[0] = new SearchBreadthFirst(this);
    m_Search[1] = new SearchDepthFirst(this);
    m_Search[2] = new SearchA(this);

    // catch mouseclicks on maze
    this->add_events(Gdk::BUTTON_PRESS_MASK);
    this->signal_event().connect(
	sigc::mem_fun(*this, &Maze::on_event_happend)
    );
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
    return (getBlock(i_id) == NULL) ? false : true;
}

/* get number of blocks
 */
int Maze::getMaxId() { return m_blocks.size(); }

/* get Block object with certain id
 * @param int: id
 */
Block* Maze::getBlock(int i_id) {
    return (m_blocks.find(i_id) != m_blocks.end()) ? m_blocks[i_id] : NULL;
}

/* getter and setter */
bool Maze::isPaintable() { return m_is_paintable; }
void Maze::setPaintable(bool i_bool) { m_is_paintable = i_bool; }
int Maze::getColumnNum() { return m_columns; }
void Maze::setMsg(const char* i_msg) {
    cout << "Msg: " << i_msg << "\n";
    m_msg = i_msg;
    queue_draw();
}
void Maze::setSearchtype(int i_type) { m_searchtype = i_type; }

/* ############################## draw ###################################### */

/* draw
 */
bool Maze::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {

    // get current width and height
    Gtk::Allocation allocation = get_allocation();
    m_maze_width = allocation.get_width();
    m_maze_height = allocation.get_height() - SIZEOFLEGEND;

    // get size of blocks
    m_block_width = m_maze_width / m_columns;
    m_block_height = m_maze_height / m_rows;

    // draw all blocks
    for (int i = 0; i < (int) m_blocks.size(); i++) {
	m_blocks[i]->drawme(cr, m_block_width, m_block_height);
    }

    // print expanded search tree
    if (!isPaintable()) {
	cr->set_line_width(5.0);
	cr->set_source_rgb(0.8, 0.0, 0.0);
	m_Search[m_searchtype]->drawTree(cr, NULL, m_block_width, m_block_height);
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

    // draw msg
    cr->move_to(m_maze_width/2+LEGENDPADDING, m_maze_height+LEGENDPADDING);
    drawText(cr, m_msg);

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
	int row = (int) (pos_y / m_block_height);
	int column = (int) (pos_x / m_block_width);

	// get id of block
	int i = (row) * m_columns + column;

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
    return m_Search[m_searchtype]->init(m_rows, m_columns);
}

/* clear search tree
 */
void Maze::clearSearch() {
    m_Search[m_searchtype]->clear();
    queue_draw();
}

/* run
 */
bool Maze::run (int) {
    cout << "Maze::run()\n";
    switch (m_Search[m_searchtype]->run()) {
	case 1:
	    m_Contr->on_menu_stop();
	    setMsg("Goal unreachable!");
	    break;
	case 2:
	    m_Contr->on_menu_pause();
	    setMsg("Reached goal!");
	    break;
	default:
	    break;
    }

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
