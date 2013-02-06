#include "AppWin.h"
#include "MyControl.h"
#include "Maze.h"

using namespace std;

/* constructor
 */
AppWin::AppWin (MyControl* i_Contr)
    : m_Contr(i_Contr)
{
    // set up window
    set_border_width(10);
    set_title("Maze");
    set_size_request(800,600);

}

/* show form to enter Maze
 */
void AppWin::askForMaze() {

    // set up structure
    m_hbox = new Gtk::HBox();
    add(*m_hbox);

    // add button
    m_button_paint = new Gtk::Button("Paint new maze");
    m_hbox->pack_start(*m_button_paint);

    // connect signals
    m_button_paint->signal_clicked().connect(sigc::mem_fun(*m_Contr, &MyControl::showPainter));

    // show
    show_all_children();
}

/* show painter
 */
void AppWin::showPainter(Maze* i_Maze) {

    m_hbox->remove(*m_button_paint);
    delete m_button_paint;
    remove();

    // create elements
    m_vbox = new Gtk::VBox();
    m_button_start = new Gtk::Button("Start/Stop");

    // connect signals
    m_button_start->signal_clicked().connect(sigc::mem_fun(*m_Contr, &MyControl::startstopMaze));

    // set up structure
    add(*m_vbox);
    m_hbox->pack_start(*m_button_start);
    m_vbox->pack_start(*i_Maze);
    m_vbox->pack_end(*m_hbox, Gtk::PACK_SHRINK, 0);

    // show
    show_all_children();
}

void AppWin::drawMenu () {

}
