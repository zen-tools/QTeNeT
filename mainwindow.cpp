#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    id = "-----";
    AlarmAt = 0;
    windowsVisible = true;

    Qt::WindowFlags flags = Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    pdialog = new PropertiesDialog(this);
    pdialog->setAttribute(Qt::WA_QuitOnClose, false);
    // Window Icon
    icon = QIcon(":/tnt.png");
    setWindowIcon(icon);
    // Tray Menu and Item's
    trayIcon = new QSystemTrayIcon(icon, this);
    trayIconMenu = new QMenu(this);
    // Tray Menu: Exit
    mAboutAction = new QAction(QIcon(":/about.png"),
                                  tr("About"), this);
    trayIconMenu->addAction(mAboutAction);

    mSettingsAction = new QAction(QIcon(":/preferences.png"),
                              tr("Settings"), this);
    trayIconMenu->addAction(mSettingsAction);

    mExitAction = new QAction(QIcon(":/exit.png"),
                              tr("Exit"), this);
    trayIconMenu->addAction(mExitAction);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip( tr(windowTitle().toLocal8Bit()) );
    trayIcon->show();

    labelStatus = new QLabel();
    ui->statusBar->addWidget(labelStatus, 0);

    LoadSettings();

    QStringList vars;
    vars << login << password << url;
    tenetXML = new ParseXML(vars);
    // Quit Shortcut Signal
    quitShortcut = new QShortcut(QKeySequence::Quit,this);
    quitShortcut->connect(quitShortcut,SIGNAL(activated()),this,SLOT(quit()));
    // Tray Signals
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(mSettingsAction, SIGNAL(triggered()), this, SLOT(clickPropertiesButton()));
    connect(mExitAction, SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(quit()));
    connect(ui->aboutButton,SIGNAL(clicked()),this,SLOT(about()));
    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(clickUpdateButton()));
    connect(ui->propertiesButton, SIGNAL(clicked()), this, SLOT(clickPropertiesButton()));

    connect(tenetXML, SIGNAL(saldo(double)), ui->lcdNumber, SLOT(display(double)));
    connect(tenetXML, SIGNAL(result(QString)), labelStatus, SLOT(setText(QString)));
    connect(tenetXML, SIGNAL(id(QString)), this, SLOT(setID(QString)));

    QTimer::singleShot(HALF_MINUTE, this, SLOT(timerFinished()));
}

MainWindow::~MainWindow() {
    delete pdialog;
    delete quitShortcut;
    delete labelStatus;
    delete trayIcon;
    delete trayIconMenu;
    delete tenetXML;
    delete ui;
}

void MainWindow::LoadSettings() {
    QByteArray m_geometry;
    QSettings qtenet("QTenet", "qtenet");
    qtenet.beginGroup("MainWindow");
    m_geometry = qtenet.value("geometry", m_geometry).toByteArray();
    if (m_geometry.size() != 0) {
        restoreGeometry(m_geometry);
    }
    windowsVisible = qtenet.value("startUpFlag", this->isVisible()).toBool();
    (windowsVisible) ? hide() : show();
    pdialog->setStartUpFlag(windowsVisible);
    qtenet.endGroup();

    qtenet.beginGroup("Authorization");
    login = qtenet.value("login", login).toString();
    password = qtenet.value("password", password).toString();

    url = qtenet.value("url", url).toString();
    if (url == "")
        url = QString("https://stats.tenet.ua/utl/!gadgapi.ls_state_evpkt");
    qtenet.endGroup();

    qtenet.beginGroup("Alarm");
    AlarmAt = qtenet.value("AlarmAt", AlarmAt).toDouble();
    qtenet.endGroup();

    pdialog->setLogin(login);
    pdialog->setPassword(password);
    pdialog->setAlertAt(AlarmAt);
}

void MainWindow::SaveSettings() {
    QSettings qtenet("QTenet", "qtenet");
    qtenet.beginGroup("MainWindow");
    qtenet.setValue("geometry", saveGeometry());
    qtenet.setValue("startUpFlag", pdialog->getStartUpFlag());
    qtenet.endGroup();

    qtenet.beginGroup("Authorization");
    qtenet.setValue("login", login);
    qtenet.setValue("password", password);
    qtenet.setValue("url", url);
    qtenet.endGroup();

    qtenet.beginGroup("Alarm");
    qtenet.setValue("AlarmAt", AlarmAt);
    qtenet.endGroup();
}

void MainWindow::about() {
    About about(this);
    about.setAttribute(Qt::WA_QuitOnClose, false);
    about.exec();
}

void MainWindow::clickPropertiesButton() {
    if (pdialog->exec() == true) {
        login = pdialog->getLogin();
        password = pdialog->getPassword();
        AlarmAt = pdialog->getAlertAt();
        windowsVisible = pdialog->getStartUpFlag();
        QStringList vars;
        vars << login << password << url;
        tenetXML->updateVars(vars);
        SaveSettings();
    } else {
        pdialog->setLogin(login);
        pdialog->setPassword(password);
        pdialog->setAlertAt(AlarmAt);
        pdialog->setStartUpFlag(windowsVisible);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    hide();
    event->ignore();
}

void MainWindow::clickUpdateButton() {
    tenetXML->replayRequest();
}

void MainWindow::quit() {
    trayIcon->hide();
    SaveSettings();
    exit(0);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            isVisible() ? hide() : showNormal();
        break;
        case QSystemTrayIcon::MiddleClick:
            showMessage(id, ui->lcdNumber->value());
        break;
        default:
            return;
        break;
    }
}

void MainWindow::showMessage(QString id, double saldo) {
    if (id == "-----") {
        QString title = tr("QTeNeT");
        QString subj = tr("Failed to get billing information");

        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage(title, subj, icon, 5 * 1000);
    } else {
        QString title(tr("Status of the account #"));
        title.append(id);

        QString subj = tr("Your account balance is ");
        subj.append(QString::number(saldo));
        subj.append(" ");
        subj.append(tr("uah"));

        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage(title, subj, icon, 5 * 1000);
    }
}

void MainWindow::setID(QString string) { id = string; }

void MainWindow::timerFinished() {
    tenetXML->replayRequest();

    if (AlarmAt >= ui->lcdNumber->value())
        showMessage(id, ui->lcdNumber->value());
}
