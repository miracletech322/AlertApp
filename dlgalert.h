#ifndef DLGALERT_H
#define DLGALERT_H

#include <QDialog>
#include <QScreen>
#include <QRect>
#include <QSoundEffect>
#include <QCloseEvent>

namespace Ui {
class DlgAlert;
}

class DlgAlert : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAlert(QString strUser, QString strMac, QWidget *parent = nullptr);
    ~DlgAlert();

public slots:
    void on_btnAcknowledge_clicked();
    void on_btnSend_clicked();

protected:
    void closeEvent(QCloseEvent *e);

private:
    Ui::DlgAlert *ui;

    QSoundEffect *sound;
    QString m_strMac;
    QString m_strUser;
};

#endif // DLGALERT_H
