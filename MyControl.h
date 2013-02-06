#ifndef _MYCONTROL_H
#define _MYCONTROL_H

#include <gtkmm.h>
#include "Maze.h"
#include "AppWin.h"

class MyControl {

    public:
	MyControl();
	AppWin* getAppWin();
	Maze* getMaze();

	void showPainter();
	void startstopMaze();
	void runMaze();
	void stopMaze();

	//Signal handlers:
	void on_menu_new();
	void on_menu_quit();
	void on_menu_file_save();
	void on_menu_file_load();

    protected:
	Maze* m_Maze;
	AppWin* m_AppWin;

	sigc::connection m_timer;
	bool m_running;
	bool m_paintview;
};

#endif // _MYCONTROL_H
