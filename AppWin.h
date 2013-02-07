#ifndef _APPWIN_H
#define _APPWIN_H

#include <gtkmm.h>
#include <string>

class MyControl;
class Maze;

class AppWin : public Gtk::Window {

    public:
	AppWin(MyControl* i_Contr);
	void showPainter(Maze* i_Maze);
	std::string getPath(Gtk::FileChooserAction type, const Gtk::BuiltinStockID button);

    protected:
	MyControl* m_Contr;

	Gtk::Button* m_button_paint;
	Gtk::VBox* m_vbox;

	Gtk::Dialog* m_dialog;

	Glib::RefPtr<Gtk::UIManager> m_refUIManager;
	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	void drawMenu();
};

#endif // _APPWIN_H
