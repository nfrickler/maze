#ifndef _MYCONTROL_H
#define _MYCONTROL_H

#include <gtkmm.h>
#include <string>
#include "Maze.h"
#include "AppWin.h"

class MyControl {

    public:
	MyControl();
	AppWin* getAppWin();
	Maze* getMaze();
	void addExtension(std::string* path);

	void runMaze();
	void stopMaze();

	bool initSearch();
	void clearSearch();

	void startTimer();
	void stopTimer();

	//Signal handlers:
	void on_menu_new();
	void on_menu_quit();
	void on_menu_file_save();
	void on_menu_file_save_as();
	void on_menu_file_load();
	void on_menu_run();
	void on_menu_stop();
	void on_menu_pause();

	void on_menu_type0();
	void on_menu_type1();

    protected:
	Maze* m_Maze;
	AppWin* m_AppWin;

	sigc::connection m_timer;
	bool m_paused;
	bool m_running;
	std::string m_path;
};

#endif // _MYCONTROL_H
