#include "MyControl.h"
#include <iostream>
#include <gtkmm/application.h>

using namespace std;

int main (int argc, char** argv) {
    cout << "\n----- Maze -----\n\n";

    // start gtkmm
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    // create MyControl object
    MyControl* Contr = new MyControl();

    // run
    app->run(*(Contr->getAppWin()));

    return 0;
}
