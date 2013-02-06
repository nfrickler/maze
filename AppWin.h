#ifndef _APPWIN_H
#define _APPWIN_H

#include <gtkmm.h>

class MyControl;
class Maze;

class AppWin : public Gtk::Window {

    public:
	AppWin(MyControl* i_Contr);
	void askForMaze();
	void showPainter(Maze* i_Maze);

    protected:
	MyControl* m_Contr;

	Gtk::Button* m_button_paint;
	Gtk::Button* m_button_start;
	Gtk::HBox* m_hbox;
	Gtk::VBox* m_vbox;

	void drawMenu();
};

#endif // _APPWIN_H
