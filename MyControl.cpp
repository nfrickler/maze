#include "MyControl.h"
#include <iostream>
#include <string>

using namespace std;

/* constructor
 */
MyControl::MyControl () {

    // init
    m_running = false;
    m_paused = false;

    // create new Maze object
    m_Maze = new Maze(this);

    // create new AppWin object
    m_AppWin = new AppWin(this);

    // show Window
    m_AppWin->showPainter(m_Maze);
    on_menu_new();
}

/* getter */
AppWin* MyControl::getAppWin () { return m_AppWin; }
Maze* MyControl::getMaze () { return m_Maze; }

/* ################### new/quit/load/save ################## */

/* create new emtpy maze
 */
void MyControl::on_menu_new() {
    cout << "MyControl:on_menu_new()\n";
    m_Maze->setMsg("Create/Load new maze");
    stopTimer();
    m_path = "";

    int rownum = m_AppWin->getRowNum();
    int colnum = m_AppWin->getColNum();
    if (rownum > 100 || rownum < 0) rownum = 12;
    if (colnum > 100 || colnum < 0) colnum = 12;
    m_Maze->createMaze(rownum, colnum);
    m_Maze->setPaintable(true);
}

/* quit
 */
void MyControl::on_menu_quit() {
    cout << "MyControl:on_menu_quit()\n";
    m_AppWin->hide();
}

/* save maze
 */
void MyControl::on_menu_file_save() {
    cout << "MyControl:on_menu_file_save()\n";

    // is path?
    if (m_path.length() == 0) {
	m_path = m_AppWin->getPath(Gtk::FILE_CHOOSER_ACTION_SAVE, Gtk::Stock::SAVE);
    }
    if (m_path.length() == 0) return;

    // add extension
    addExtension(&m_path);

    // save
    m_Maze->saveMaze(&m_path);
}

/* save maze as
 */
void MyControl::on_menu_file_save_as() {
    cout << "MyControl:on_menu_file_save_as()\n";

    // get path
    m_path = m_AppWin->getPath(Gtk::FILE_CHOOSER_ACTION_SAVE, Gtk::Stock::SAVE);
    if (m_path.length() == 0) return;

    // add extension
    addExtension(&m_path);

    // save
    m_Maze->saveMaze(&m_path);
}

/* load maze from file
 */
void MyControl::on_menu_file_load() {
    cout << "MyControl:on_menu_file_load()\n";
    stopTimer();

    // get path
    m_path = m_AppWin->getPath(Gtk::FILE_CHOOSER_ACTION_OPEN, Gtk::Stock::OPEN);
    if (m_path.length() == 0) return;

    // load maze
    m_Maze->setMsg("Create/Load new maze");
    m_Maze->loadMaze(&m_path);
    m_Maze->setPaintable(true);
}

/* ################### run/pause/stop ################## */

/* run search
 */
void MyControl::on_menu_run() {
    cout << "MyControl:on_menu_run()\n";
    if (!initSearch()) return;
    m_Maze->setMsg("Running...");
    startTimer();
    m_Maze->setPaintable(false);
}

/* stop search
 */
void MyControl::on_menu_stop() {
    cout << "MyControl:on_menu_stop()\n";
    m_Maze->setMsg("Stopped");
    stopTimer();
    clearSearch();
    m_Maze->setPaintable(true);
}

/* pause search
 */
void MyControl::on_menu_pause() {
    cout << "MyControl:on_menu_pause()\n";
    if (m_running) {
	m_Maze->setMsg("Paused");
	stopTimer();
	m_paused = true;
    } else if (m_paused) {
	m_Maze->setMsg("Running...");
	startTimer();
    } else {
	cerr << "ERROR: Search is not paused!\n";
    }
}

/* ################### change search type ################## */

/* change to search type 0
 */
void MyControl::on_menu_type0() {
    cout << "MyControl:on_menu_type0()\n";
    initSearch();
    m_Maze->setSearchtype(0);
}

/* change to search type 1
 */
void MyControl::on_menu_type1() {
    cout << "MyControl:on_menu_type1()\n";
    initSearch();
    m_Maze->setSearchtype(1);
}

/* change to search type 2
 */
void MyControl::on_menu_type2() {
    cout << "MyControl:on_menu_type2()\n";
    initSearch();
    m_Maze->setSearchtype(2);
}

/* ################### timer handling ################## */

/* start/stop timer
 */
void MyControl::startTimer() {
    m_running = true;
    m_paused = false;
    sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*m_Maze, &Maze::run),0);
    m_timer = Glib::signal_timeout().connect(my_slot, 1000);
}
void MyControl::stopTimer() {
    m_running = false;
    m_paused = false;
    m_timer.disconnect();
}

/* ################### search handling ################## */

/* init/clear search
 */
bool MyControl::initSearch () {
    clearSearch();
    if (!m_Maze->initSearch()) {
	cerr << "MyControl::initSearch(): ERROR\n";
	return false;
    }
    return true;
}
void MyControl::clearSearch () {
    m_Maze->clearSearch();
}

/* ################### misc ################## */

/* add extension to file if missing
 */
void MyControl::addExtension(std::string* path) {

    // is extension?
    string ext = ".maze";
    if (path->rfind(ext, string::npos) != (path->size() - 5)) {
	// add extension
	*path += ext;
    }
}

