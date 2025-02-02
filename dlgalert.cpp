#include "dlgalert.h"
#include "ui_dlgalert.h"
#include "mainwindow.h"

DlgAlert::DlgAlert(QString strUser, QString strMac, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgAlert)
{
    ui->setupUi(this);

    ui->lblUser->setText(strUser);
    m_strMac = strMac;
    m_strUser = strUser;

    QScreen *screen = QApplication::primaryScreen();
    if (screen)
    {
        QRect screenGeometry = screen->geometry();
        setGeometry(screenGeometry);
    }

    sound = new QSoundEffect(this);  
    sound->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/assets/alarm-sound.wav"));
    sound->setLoopCount(QSoundEffect::Infinite);
    sound->play();
}

DlgAlert::~DlgAlert()
{
    delete ui;

    sound->stop();
    sound = nullptr;
    delete sound;
}

void DlgAlert::on_btnActknowledge_clicked()
{
    close();
}

void DlgAlert::on_btnSend_clicked()
{
    MainWindow::getInstance()->on_lblLogo_clicked();
    close();
}

void DlgAlert::closeEvent(QCloseEvent *e)
{
    MainWindow::getInstance()->receivedAlert(m_strUser, m_strMac);

    sound->stop();
    sound = nullptr;
    delete sound;

    e->accept();
}
