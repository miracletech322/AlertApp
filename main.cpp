#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QByteArray>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QProcess process;
    process.start("taskkill", QStringList() << "/f" << "/im" << "GPShield.exe");
    process.waitForFinished();
    QByteArray errorOutput = process.readAllStandardError();
    qDebug() << errorOutput;

    MainWindow::getInstance()->show();
    QIcon icon(":/Resource/assets/logo.png");
    MainWindow::getInstance()->setWindowIcon(icon);
    return a.exec();
}
