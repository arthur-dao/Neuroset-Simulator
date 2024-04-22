#ifndef PCWINDOW_H
#define PCWINDOW_H

#include <QMainWindow>
#include "Session.h"
#include "Frequency.h"

/**
 * @file PCWindow.h
 * @brief Declaration of the PCWindow class.
 * 
 * This file contains the declaration of the PCWindow class, which represents
 * the window interface for the PC application used to manage sessions and uploaded data.
 * It includes methods for displaying sessions and handling new uploads.
 */

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
