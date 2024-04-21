#include "pcwindow.h"
#include "ui_pcwindow.h"

PCWindow::PCWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PCWindow)
{
    ui->setupUi(this);
}

PCWindow::~PCWindow()
{
    delete ui;
}

void PCWindow::displaySessions(QList<Session> sessions){
    QString displayText;
    for(Session s : sessions){
        displayText += s.toString();
    }

    ui->textBrowser->setText(displayText);
}

void PCWindow::newUpload(Session newSession){
    ui->textBrowser->append(newSession.toString());
//    QList<Frequency> list = newSession.getFrequencies();
//    for(Frequency f : list){
//        ui->textBrowser->append("Initial: " + QString::number(f.getInitialFrequency()) + " - Final: " +  QString::number(f.getFinalFrequency()));
//    }
}
