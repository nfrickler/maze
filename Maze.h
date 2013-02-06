#ifndef _MAZE_H
#define _MAZE_H

#include <gtkmm/drawingarea.h>
#include <map>
#include "Block.h"

class MyControl;

typedef struct s_element {
    Block* block;
    int root;
    int sum;
    struct s_element* next;
} t_element;

class Maze : public Gtk::DrawingArea {

    public:
	Maze(MyControl* i_Contr);
	void create(int i_columns, int i_rows);
	void deleteOld();
	bool on_event_happend(GdkEvent *event);

	bool isPaintable();
	void setPaintable(bool i_bool);
	bool run(int);
	bool initSearch();
	bool isBlock (int i_id);

	t_element* createNode(Block* i_Block);
	bool expandNode(t_element*);
	void addToList (t_element* newone);

    protected:
	MyControl* m_Contr;
	int m_columns;
	int m_rows;
	int m_maze_width, m_maze_height;
	std::map<int,Block*> m_blocks;
	bool m_is_paintable;

	// searching
	t_element* m_list;
	t_element* m_moved_from;
	t_element* m_moved_to;
	std::map<int, std::map<int,t_element*> > m_lines;

	// drawing methods
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void drawText(const Cairo::RefPtr<Cairo::Context>& cr, const char text[]);
};

#endif // _MAZE_H
