#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QShortcut>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QStringList>
#include <QList>
#include <QTimer>
#include <QCloseEvent>
#include <QDebug>
#include "propertiesdialog.h"
#include "parsexml.h"
#include "about.h"

#define HALF_MINUTE 30000


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    QLabel *labelStatus;
    QShortcut *quitShortcut;
    PropertiesDialog *pdialog;
    QSystemTrayIcon *trayIcon;
    QAction *mExitAction,
            *mSettingsAction,
            *mAboutAction,
            *mStatusAction;
    QMenu *trayIconMenu;
    ParseXML *tenetXML;
    QIcon icon;
    QString id;
    QString login;
    QString password;
    QString url;
    double AlarmAt;
    bool windowsVisible;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void LoadSettings();
    void SaveSettings();

private slots:
    void about();
    void clickPropertiesButton();
    void clickUpdateButton();
    void quit();
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void showMessage(QString, double);
    void setID(QString);
    void timerFinished();

private:
    Ui::MainWindow *ui;
    virtual void closeEvent(QCloseEvent *);

};

#endif // MAINWINDOW_H
