#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

PropertiesDialog::PropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertiesDialog) {
    ui->setupUi(this);
}

PropertiesDialog::~PropertiesDialog() {
    delete ui;
}

QString PropertiesDialog::getLogin() { return ui->loginLineEdit->text(); }

QString PropertiesDialog::getPassword() {
    if (ui->passwordLineEdit->isModified()) {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(ui->passwordLineEdit->text().toAscii()); 
        QString hashStr(hash.result().toHex());
        return hashStr;
    }
    return ui->passwordLineEdit->text();
}

double PropertiesDialog::getAlertAt() { return ui->alertSpinBox->value(); }

bool PropertiesDialog::getStartUpFlag() { return ui->startUp->isChecked(); }

void PropertiesDialog::setLogin(QString lg) { ui->loginLineEdit->setText(lg); }

void PropertiesDialog::setPassword(QString p) { ui->passwordLineEdit->setText(p); }

void PropertiesDialog::setAlertAt(double alert) { ui->alertSpinBox->setValue(alert); }

void PropertiesDialog::setStartUpFlag(bool flag) { ui->startUp->setChecked(flag);}
