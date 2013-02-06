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
    m_Maze->create(12, 12);

    // create new AppWin object
    m_AppWin = new AppWin(this);

    // ask user for new Maze
    m_AppWin->askForMaze();
}

/* getter */
AppWin* MyControl::getAppWin () { return m_AppWin; }
Maze* MyControl::getMaze () { return m_Maze; }

/* show painter
 */
void MyControl::showPainter() {
    cout << "MyControl::showPainter()\n";

    // show painter
    if (!m_paintview) {
	m_paintview = true;
	m_AppWin->showPainter(m_Maze);
    }

    // make paintable
    m_Maze->setPaintable(true);
}

/* slot for switching status of timer
 */
void MyControl::startstopMaze () {
    if (m_running) {
	m_running = false;
	m_Maze->setPaintable(true);
	stopMaze();
    } else {
	m_running = true;
	m_Maze->setPaintable(false);
	runMaze();
    }
}

/* run Maze
 */
void MyControl::runMaze() {
    cout << "MyControl::runMaze()\n";

    // init search
    if (!m_Maze->initSearch()) {
	cout << "MyControl::runMaze(): ERROR\n";
	showPainter();
    }

    // start timer
    sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*m_Maze,
	&Maze::run),0);
    m_timer = Glib::signal_timeout().connect(my_slot, 1000);
}

/* stop timer
 */
void MyControl::stopMaze() {
    cout << "MyControl::stopMaze()\n";
    m_timer.disconnect();

    // back to painter
    showPainter();
}
