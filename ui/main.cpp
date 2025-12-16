#include "mainwindow.h"

#include <QApplication>

// Main entry point for BASIC interpreter GUI application
// Creates Qt application instance, shows main window, and starts event loop
int main(int argc, char *argv[])
{
    // Create Qt application object
    QApplication a(argc, argv);
    
    // Create and display main window
    MainWindow w;
    w.show();
    
    // Start event loop (blocks until application exits)
    return a.exec();
}
