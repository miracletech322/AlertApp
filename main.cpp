#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>

QSharedMemory shared("AlertApp");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!shared.create(512)) {
//        QMessageBox::warning(nullptr, "Instance Already Running", "Another instance of this application is already running.");
        return -1;
    }

    MainWindow::getInstance()->show();
    QIcon icon(":/Resource/assets/logo.png");
    MainWindow::getInstance()->setWindowIcon(icon);
    return a.exec();
}
