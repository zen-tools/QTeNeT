#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCryptographicHash>

namespace Ui {
    class PropertiesDialog;
}

class PropertiesDialog : public QDialog {
    Q_OBJECT
    QString login;
    QString password;
public:
    explicit PropertiesDialog(QWidget *parent = 0);
    ~PropertiesDialog();
    QString getLogin();
    QString getPassword();
    double getAlertAt();
    bool getStartUpFlag();
    void setLogin(QString);
    void setPassword(QString);
    void setAlertAt(double);
    void setStartUpFlag(bool);

public slots:


private:
    Ui::PropertiesDialog *ui;
};

#endif // PROPERTIESDIALOG_H
