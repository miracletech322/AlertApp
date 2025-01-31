#include "dlgsettings.h"
#include "ui_dlgsettings.h"
#include <QSettings>
#include "mainwindow.h"
#include <QMessageBox>

DlgSettings::DlgSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgSettings)
{
    ui->setupUi(this);

    QSettings settings("MiracleTech", "AlertApp");
    QString strName = settings.value("Username").toString();

    if(strName == "") {
        strName = MainWindow::getInstance()->getUsername();
        settings.setValue("Username", strName);
    }

    ui->edtUser->setText(strName);
}

DlgSettings::~DlgSettings()
{
    delete ui;
}

void DlgSettings::on_btnSave_clicked()
{
    QString strName = ui->edtUser->text();
    if(strName == "") {
        QMessageBox::information(this, "Warning", "Please input valid username");
        return;
    }

    QSettings settings("MiracleTech", "AlertApp");

    settings.setValue("Username", strName);
    close();
}

void DlgSettings::on_btnClose_clicked()
{
    close();
}
