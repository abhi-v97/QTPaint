#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // start the application
    QApplication app(argc, argv);

    // create the window
    MainWindow window;
    window.show();

    // display the window
    return app.exec();
}
