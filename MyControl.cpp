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
    m_Maze->createMaze(12, 12);
    m_Maze->setPaintable(true);

    // create new AppWin object
    m_AppWin = new AppWin(this);

    // show Window
    m_AppWin->showPainter(m_Maze);
}

/* getter */
AppWin* MyControl::getAppWin () { return m_AppWin; }
Maze* MyControl::getMaze () { return m_Maze; }

/* create new emtpy maze
 */
void MyControl::on_menu_new() {
    cout << "MyControl:on_menu_new()\n";
    stopTimer();
    m_path = "";
    m_Maze->createMaze(12, 12);
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
    m_Maze->loadMaze(&m_path);
    m_Maze->setPaintable(true);
}

/* run search
 */
void MyControl::on_menu_run() {
    cout << "MyControl:on_menu_run()\n";
    if (!initSearch()) return;
    startTimer();
    m_Maze->setPaintable(false);
}

/* stop search
 */
void MyControl::on_menu_stop() {
    cout << "MyControl:on_menu_stop()\n";
    stopTimer();
    clearSearch();
    m_Maze->setPaintable(true);
}

/* pause search
 */
void MyControl::on_menu_pause() {
    cout << "MyControl:on_menu_pause()\n";
    if (m_running) {
	stopTimer();
	m_paused = true;
    } else if (m_paused) {
	cout << "start it\n";
	startTimer();
    } else {
	cout << "ERROR: Search is not paused!\n";
    }
}

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

/* init/clear search
 */
bool MyControl::initSearch () {
    clearSearch();
    if (!m_Maze->initSearch()) {
	cout << "MyControl::initSearch(): ERROR\n";
	return false;
    }
    return true;
}
void MyControl::clearSearch () {
    m_Maze->clearSearch();
}

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

