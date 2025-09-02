#include "gui/MainWindow.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Business Scraper");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Your Company");
    app.setOrganizationDomain("yourcompany.com");

    // Set application icon (if you have one)
    // app.setWindowIcon(QIcon(":/icons/app_icon.png"));

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
