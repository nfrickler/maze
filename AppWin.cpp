#include "AppWin.h"
#include "MyControl.h"
#include "Maze.h"
#include <iostream>

using namespace std;

/* constructor
 */
AppWin::AppWin (MyControl* i_Contr)
    : m_Contr(i_Contr)
{
    // set up window
    set_border_width(0);
    set_title("Maze");
    set_size_request(800,600);

    // add toplevel box on window
    m_vbox = new Gtk::VBox();
    add(*m_vbox);

    // draw menu
    drawMenu();
}

/* show painter
 */
void AppWin::showPainter(Maze* i_Maze) {

    // set up structure
    m_vbox->pack_start(*i_Maze);

    // show
    show_all_children();
}

void AppWin::drawMenu () {

    // create actions for menus and toolbars
    m_refActionGroup = Gtk::ActionGroup::create();

    // file menu
    m_refActionGroup->add(
	Gtk::Action::create("FileNew", Gtk::Stock::NEW, "_New", "Create a new Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_new));
    m_refActionGroup->add(
	Gtk::Action::create("FileLoad", Gtk::Stock::NEW, "Load Maze", "Load Maze from file"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_file_load));
    m_refActionGroup->add(
	Gtk::Action::create("FileSave", Gtk::Stock::SAVE, "Save Maze", "Save Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_file_save));
    m_refActionGroup->add(
	Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_quit));
    m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));
    m_refActionGroup->add(
	Gtk::Action::create("Run", Gtk::Stock::MEDIA_PLAY, "Run Maze", "Run Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_run));
    m_refActionGroup->add(
	Gtk::Action::create("Pause", Gtk::Stock::MEDIA_PAUSE, "Pause Maze", "Pause Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_pause));
    m_refActionGroup->add(
	Gtk::Action::create("Stop", Gtk::Stock::MEDIA_STOP, "Stop Maze", "Stop Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_stop));

    m_refUIManager = Gtk::UIManager::create();
    m_refUIManager->insert_action_group(m_refActionGroup);
    add_accel_group(m_refUIManager->get_accel_group());

    // layout the actions in a menubar and toolbar
    Glib::ustring ui_info =
	"<ui>"
	"  <menubar name='MenuBar'>"
	"    <menu action='FileMenu'>"
	"      <menuitem action='FileNew'/>"
	"      <menuitem action='FileLoad'/>"
	"      <menuitem action='FileSave'/>"
	"      <menuitem action='FileQuit'/>"
	"    </menu>"
	"  </menubar>"
	"  <toolbar  name='ToolBar'>"
	"    <toolitem action='FileNew'/>"
	"    <toolitem action='Run'/>"
	"    <toolitem action='Pause'/>"
	"    <toolitem action='Stop'/>"
	"    <toolitem action='FileQuit'/>"
	"  </toolbar>"
	"</ui>";

    try {
	m_refUIManager->add_ui_from_string(ui_info);
    } catch(const Glib::Error& ex) {
	cerr << "Building menus failed: " <<  ex.what();
    }

    // add menubar and toolbar to vbox
    Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
    if(pMenubar) m_vbox->pack_start(*pMenubar, Gtk::PACK_SHRINK);
    Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
    if(pToolbar) m_vbox->pack_start(*pToolbar, Gtk::PACK_SHRINK);
}
