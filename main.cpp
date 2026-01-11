#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application metadata
    app.setApplicationName("Hangman Game");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Hangman Studios");

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
