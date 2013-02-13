#ifndef _MAZE_H
#define _MAZE_H

#include <gtkmm/drawingarea.h>
#include <map>
#include <string>

const int SIZEOFLEGEND = 70;
const int LEGENDPADDING = 5;
const int LEGEND_BLOCK_POSY = 35;
const int LEGEND_BLOCK_WIDTH = 30;
const int LEGEND_BLOCK_HEIGHT = 20;
const int LEGEND_BLOCK_SPACE = 120;
const int BLOCK_NAMES_SIZE = 4;

extern const Gdk::Color BLOCK_COLORS[BLOCK_NAMES_SIZE];
extern const char* BLOCK_NAMES[BLOCK_NAMES_SIZE];

class MyControl;
class Block;

typedef struct s_element {
    Block* block;
    int sum;
    struct s_element* root;
    struct s_element* subs[4];
    struct s_element* next;
} t_element;

class Maze : public Gtk::DrawingArea {

    public:
	Maze(MyControl* i_Contr);

	// create/delete maze
	void createMaze(int i_columns, int i_rows);
	void deleteMaze();

	// search
	bool initSearch();
	void clearSearch();
	t_element* createNode(Block* i_Block);
	bool expandNode(t_element*);
	void addToExpandList(t_element* newone);
	bool run(int);
	void removeTree(t_element* current);

	// gereral
	bool isPaintable();
	void setPaintable(bool i_bool);
	bool isBlock (int i_id);
	bool isBlockExpanded(t_element* current, Block* block);
	bool on_event_happend(GdkEvent *event);

	bool saveMaze(const std::string* path);
	bool loadMaze(const std::string* path);

    protected:
	MyControl* m_Contr;

	// maze
	std::map<int,Block*> m_blocks;
	int m_columns;
	int m_rows;
	bool m_is_paintable;

	// drawing
	int m_maze_width, m_maze_height;
	double m_block_width;
	double m_block_height;

	// searching
	t_element* m_stree;
	t_element* m_sexpand;

	// drawing methods
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void drawText(const Cairo::RefPtr<Cairo::Context>& cr, const char text[]);
	void drawTree(const Cairo::RefPtr<Cairo::Context>& cr, t_element* current);
};

#endif // _MAZE_H
