#include "AppWin.h"
#include "MyControl.h"
#include "Maze.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* constructor
 */
AppWin::AppWin (MyControl* i_Contr)
    : m_Contr(i_Contr)
{
    // set up window
    set_border_width(0);
    set_title("Maze");
    set_default_size(800,600);

    // init
    m_rownum = NULL;
    m_colnum = NULL;

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
	Gtk::Action::create("FileLoad", Gtk::Stock::OPEN, "Load Maze", "Load Maze from file"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_file_load));
    m_refActionGroup->add(
	Gtk::Action::create("FileSave", Gtk::Stock::SAVE, "Save Maze", "Save Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_file_save));
    m_refActionGroup->add(
	Gtk::Action::create("FileSaveAs", Gtk::Stock::SAVE_AS, "Save Maze as", "Save Maze as"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_file_save_as));
    m_refActionGroup->add(
	Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_quit));
    m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));

    // search menu
    m_refActionGroup->add(Gtk::Action::create("SearchMenu", "Search"));
    m_refActionGroup->add(
	Gtk::Action::create("Run", Gtk::Stock::MEDIA_PLAY, "Run Maze", "Run Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_run));
    m_refActionGroup->add(
	Gtk::Action::create("Pause", Gtk::Stock::MEDIA_PAUSE, "Pause Maze", "Pause Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_pause));
    m_refActionGroup->add(
	Gtk::Action::create("Stop", Gtk::Stock::MEDIA_STOP, "Stop Maze", "Stop Maze"),
	sigc::mem_fun(*m_Contr, &MyControl::on_menu_stop));

    // selection of search type
    m_refActionGroup->add(Gtk::Action::create("ChoicesMenu", "Search type"));
    Gtk::RadioAction::Group group_type;
    m_refChoice0 = Gtk::RadioAction::create(group_type, "Type0", "Breadth-First-Search");
    m_refActionGroup->add(
	m_refChoice0,
	sigc::mem_fun(*this, &AppWin::handleChoices)
    );
    m_refChoice1 = Gtk::RadioAction::create(group_type, "Type1", "Depth-First-Search");
    m_refActionGroup->add(
	m_refChoice1,
	sigc::mem_fun(*this, &AppWin::handleChoices)
    );
    m_refChoice2 = Gtk::RadioAction::create(group_type, "Type2", "A*-Search");
    m_refActionGroup->add(
	m_refChoice2,
	sigc::mem_fun(*this, &AppWin::handleChoices)
    );

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
	"      <menuitem action='FileSaveAs'/>"
	"      <menuitem action='FileQuit'/>"
	"    </menu>"
	"    <menu action='SearchMenu'>"
	"      <menuitem action='Run'/>"
	"      <menuitem action='Stop'/>"
	"      <menuitem action='Pause'/>"
	"    </menu>"
	"    <menu action='ChoicesMenu'>"
	"      <menuitem action='Type0'/>"
	"      <menuitem action='Type1'/>"
	"      <menuitem action='Type2'/>"
	"    </menu>"
	"  </menubar>"
	"  <toolbar  name='ToolBar'>"
	"    <toolitem action='FileNew'/>"
	"    <toolitem action='FileLoad'/>"
	"    <toolitem action='FileSave'/>"
	"    <toolitem action='FileSaveAs'/>"
	"    <separator/>"
	"    <toolitem action='Run'/>"
	"    <toolitem action='Pause'/>"
	"    <toolitem action='Stop'/>"
	"    <separator/>"
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
    Gtk::HBox* menuhbox = new Gtk::HBox();
    m_vbox->pack_start(*menuhbox, Gtk::PACK_SHRINK);
    Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
    if(pToolbar) menuhbox->pack_start(*pToolbar, Gtk::PACK_EXPAND_WIDGET);

    // create input fields
    Gtk::Label* sizelabel = new Gtk::Label("Size of maze:");
    menuhbox->pack_start(*sizelabel, Gtk::PACK_SHRINK);
    m_rownum = new Gtk::Entry();
    m_rownum->set_text("12");
    m_rownum->set_width_chars(3);
    m_rownum->set_max_length(3);
    menuhbox->pack_start(*m_rownum, Gtk::PACK_SHRINK);
    Gtk::Label* sizelabel2 = new Gtk::Label("x");
    menuhbox->pack_start(*sizelabel2, Gtk::PACK_SHRINK);
    m_colnum = new Gtk::Entry();
    m_colnum->set_text("12");
    m_colnum->set_width_chars(3);
    m_colnum->set_max_length(3);
    menuhbox->pack_start(*m_colnum, Gtk::PACK_SHRINK);
}

/* handle choices
 */
void AppWin::handleChoices () {

    if (m_refChoice0->get_active()) {
	m_Contr->on_menu_type0();
    } else if (m_refChoice1->get_active()) {
	m_Contr->on_menu_type1();
    } else if (m_refChoice2->get_active()) {
	m_Contr->on_menu_type2();
    }
}

/* get rownum entered
 */
int AppWin::getRowNum() {
    int out;
    stringstream ss;
    Glib::ustring cache = m_rownum->get_text();
    ss << cache.raw();
    ss >> out;
    return out;
}

/* get colnum entered
 */
int AppWin::getColNum() {
    int out;
    stringstream ss;
    Glib::ustring cache = m_colnum->get_text();
    ss << cache.raw();
    ss >> out;
    return out;
}

/* ask user for path
 */
std::string AppWin::getPath(Gtk::FileChooserAction type, const Gtk::BuiltinStockID button) {

    // create dialog
    Gtk::FileChooserDialog dialog("Please choose a file", type);
    dialog.set_transient_for(*this);

    // add buttons
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(button, Gtk::RESPONSE_OK);

    // filter: maze files
    Glib::RefPtr<Gtk::FileFilter> filter_maze = Gtk::FileFilter::create();
    filter_maze->set_name("Maze files");
    filter_maze->add_pattern("*.maze");
    dialog.add_filter(filter_maze);

    // filter: all files
    Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
    filter_any->set_name("All files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    // ask user
    int result = dialog.run();

    // handle response
    switch(result) {
	case(Gtk::RESPONSE_OK): {
	    cout << "Open clicked." << endl;
	    std::string path = dialog.get_filename();
	    cout << "File selected: " <<  path << endl;
	    return path;
	}
	case(Gtk::RESPONSE_CANCEL): {
	    cout << "Cancel clicked." << endl;
	    break;
	}
	default: {
	    cout << "Unexpected button clicked." << endl;
	    break;
	}
    }

    return "";
}
