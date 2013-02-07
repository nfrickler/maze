#include "MyControl.h"
#include <iostream>

using namespace std;

/* constructor
 */
MyControl::MyControl () {

    // init
    m_running = false;
    m_paintview = false;

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

/* handle menu
 */
void MyControl::on_menu_new() {
    cout << "MyControl:on_menu_new()\n";
    m_Maze->createMaze(12, 12);
}
void MyControl::on_menu_quit() {
    cout << "MyControl:on_menu_quit()\n";
    m_AppWin->hide();
}
void MyControl::on_menu_file_save() {
    cout << "MyControl:on_menu_file_save()\n";

}
void MyControl::on_menu_file_load() {
    cout << "MyControl:on_menu_file_load()\n";

}
void MyControl::on_menu_run() {
    cout << "MyControl:on_menu_run()\n";
    if (!initSearch()) return;
    startTimer();
    m_Maze->setPaintable(false);
}
void MyControl::on_menu_stop() {
    cout << "MyControl:on_menu_stop()\n";
    stopTimer();
    clearSearch();
    m_Maze->setPaintable(true);
}
void MyControl::on_menu_pause() {
    cout << "MyControl:on_menu_pause()\n";
    if (m_running) {
	stopTimer();
    } else {
	startTimer();
    }
}

/* start/stop timer
 */
void MyControl::startTimer() {
    m_running = true;
    sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*m_Maze, &Maze::run),0);
    m_timer = Glib::signal_timeout().connect(my_slot, 1000);
}
void MyControl::stopTimer() {
    m_running = false;
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

