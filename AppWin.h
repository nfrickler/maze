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

	int getRowNum();
	int getColNum();
	void handleChoices();

    protected:
	MyControl* m_Contr;

	Gtk::Button* m_button_paint;
	Gtk::VBox* m_vbox;

	Gtk::Dialog* m_dialog;
	Gtk::Entry *m_colnum, *m_rownum;

	Glib::RefPtr<Gtk::UIManager> m_refUIManager;
	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
	Glib::RefPtr<Gtk::RadioAction> m_refChoice0, m_refChoice1, m_refChoice2;

	void drawMenu();
};

#endif // _APPWIN_H
