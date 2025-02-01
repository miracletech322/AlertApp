#include "mainwindow.h"
#include "dlgalert.h"
#include "dlgsettings.h"
#include "./ui_mainwindow.h"

#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QSettings>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSettings>

static MainWindow* instance = nullptr;

MainWindow *MainWindow::getInstance()
{
    if(instance == nullptr) {
        instance = new MainWindow(0);
    }

    return instance;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    moveToBottomRight();
    initSocket();
    initTrayIcon();
    setStartup();

    QSettings settings("MiracleTech", "AlertApp");
    QString strName = settings.value("Username").toString();

    if(strName == "") {
        settings.setValue("Username", getUsername());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lblLogo_clicked()
{
    QSettings settings("MiracleTech", "AlertApp");
    QString strName = settings.value("Username").toString();

    QString strDiagram = "SEND$" + getMacAddress() + "$" + strName;
    m_pSender->writeDatagram(strDiagram.toLocal8Bit(), QHostAddress::Broadcast, 3022);

    ui->centralwidget->setStyleSheet("QLabel#lblLogo { border-image: url(:/Resource/assets/logo-red.png); }");
}

void MainWindow::slt_readPendingDatagrams()
{
    while (m_pReceiver->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_pReceiver->receiveDatagram();
        QString strDiagram = QString::fromLocal8Bit(datagram.data());
        QStringList lst = strDiagram.split("$");

        if(lst[0] == "SEND") {
            if(lst[1] != getMacAddress()) {
                DlgAlert *pDialog = new DlgAlert(lst[2], lst[1], this);
                pDialog->showFullScreen();
            }
        }

        if(lst[0] == "RECEIVE") {
            if(lst[1] == getMacAddress()) {
                ui->centralwidget->setStyleSheet("QLabel#lblLogo { border-image: url(:/Resource/assets/logo.png); }");
            }
        }
    }
}

void MainWindow::slt_openSettings()
{
    DlgSettings *pDlg = new DlgSettings(this);
    pDlg->show();
}

void MainWindow::slt_close()
{
    close();
}

void MainWindow::initSocket()
{
    m_pSender = new QUdpSocket(this);

    m_pReceiver = new QUdpSocket(this);
    m_pReceiver->bind(QHostAddress::Any, 3022);
    connect(m_pReceiver, SIGNAL(readyRead()), this, SLOT(slt_readPendingDatagrams()));
}

void MainWindow::moveToBottomRight()
{
    QScreen *screen = QApplication::primaryScreen();
    QRect availableGeometry = screen->availableGeometry();

    int x = availableGeometry.right() - width();
    int y = availableGeometry.bottom() - height();

    move(x, y);
}

void MainWindow::receivedAlert(QString strMac)
{
    QString strDiagram = "RECEIVE$" + strMac;
    m_pSender->writeDatagram(strDiagram.toLocal8Bit(), QHostAddress::Broadcast, 3022);
}

void MainWindow::initTrayIcon()
{
    QIcon icon(":/Resource/assets/logo.png");

    m_pTrayIcon = new QSystemTrayIcon();
    m_pTrayIcon->setIcon(icon);

    QMenu *pTrayMenu = new QMenu();

    QAction *pSettingsAction = new QAction("Settings", pTrayMenu);
    connect(pSettingsAction, SIGNAL(triggered()), this, SLOT(slt_openSettings()));
    QAction *pExitAction = new QAction("Exit", pTrayMenu);
    connect(pExitAction, SIGNAL(triggered()), this, SLOT(slt_close()));

    pTrayMenu->addAction(pSettingsAction);
    pTrayMenu->addSeparator();
    pTrayMenu->addAction(pExitAction);
    m_pTrayIcon->setContextMenu(pTrayMenu);
    m_pTrayIcon->show();
}

void MainWindow::setStartup()
{
    QString appPath = QCoreApplication::applicationFilePath();
    QSettings settings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    settings.setValue("AlertApp", QDir::toNativeSeparators(appPath));
}

QString MainWindow::getUsername()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString username = env.value("USERNAME", "Unknown User");
    return username;
}

QString MainWindow::getMacAddress()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : interfaces) {
        if (interface.flags() & QNetworkInterface::IsUp && interface.flags() & QNetworkInterface::IsRunning) {
            QByteArray macAddress = interface.hardwareAddress().toUtf8();
            return QString(macAddress);
        }
    }

    return "";
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging)
    {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = false;
        event->accept();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this, "Exit App", "Do you want to exit application?") == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}
