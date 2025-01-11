#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QProcessEnvironment>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QScreen>
#include <QRect>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* getInstance();

    void initSocket();
    void sendBroadcast();
    void moveToBottomRight();
    void receivedAlert(QString strMac);

    QString getUsername();
    QString getMacAddress();

    bool m_dragging = false;
    QPoint m_dragPosition;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void on_lblLogo_clicked();
    void slt_readPendingDatagrams();

private:
    Ui::MainWindow *ui;

    QUdpSocket *m_pSender;
    QUdpSocket *m_pReceiver;
};
#endif // MAINWINDOW_H
