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
class Search;

class Maze : public Gtk::DrawingArea {

    public:
	Maze(MyControl* i_Contr);

	// create/delete maze
	void createMaze(int i_rows, int i_columns);
	void deleteMaze();

	// search
	bool initSearch();
	void clearSearch();
	bool runSearch();
	Search* m_Search[3];
	int m_searchtype;
	bool run(int);
	void setSearchtype(int);

	// gereral
	bool isPaintable();
	void setPaintable(bool i_bool);
	bool isBlock (int i_id);
	Block* getBlock(int i_id);
	int getMaxId();
	bool on_event_happend(GdkEvent *event);
	int getColumnNum();

	bool saveMaze(const std::string* path);
	bool loadMaze(const std::string* path);

	void setMsg(const char* i_msg);

    protected:
	MyControl* m_Contr;

	// maze
	std::map<int,Block*> m_blocks;
	int m_columns;
	int m_rows;
	bool m_is_paintable;
	const char* m_msg;

	// drawing
	int m_maze_width, m_maze_height;
	double m_block_width;
	double m_block_height;

	// drawing methods
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void drawText(const Cairo::RefPtr<Cairo::Context>& cr, const char text[]);
};

#endif // _MAZE_H
