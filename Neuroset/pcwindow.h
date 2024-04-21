#ifndef PCWINDOW_H
#define PCWINDOW_H

#include <QMainWindow>
#include "Session.h"
#include "Frequency.h"

namespace Ui {
class PCWindow;
}

class PCWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PCWindow(QWidget *parent = nullptr);
    ~PCWindow();

    void displaySessions(QList<Session>);
    void newUpload(Session);

private:
    Ui::PCWindow *ui;
    QList<Session> uploadedSessions;
};

#endif // PCWINDOW_H
