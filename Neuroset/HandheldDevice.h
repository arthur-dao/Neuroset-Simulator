#ifndef HANDHELDDEVICE_H
#define HANDHELDDEVICE_H

#include "defs.h"
#include "Headset.h"
#include "Session.h"

#include "QTimer"
#include "QObject"
#include "QDebug"
#include <list>

class HandheldDevice : public QObject{
    Q_OBJECT

    public:
        explicit HandheldDevice(int batteryLevel = 100);
        ~HandheldDevice();

    public slots:
        void createSession();
        void shutdown();
        void beginSession();

        void stop();
        void pause();
        void resume();

        void dateTimeSelection();
        void displaySessionLog();
        void navigateUp();
        void navigateDown();
        void select();
        void menuToggle();
        void uploadToPC();
        void updateMenu();

        void powerToggle();
        void reduceBattery();
        void chargeBatteryToFull();
        void disconnect();

    private:
        DeviceStatus deviceStatus;
        RunStatus runStatus;
        int batteryPercent;
        bool contact;
        bool treatmentSig;

        QTimer stopTimer;
        QTimer runTimer;

        std::list<Session> sessions;
        static const int size = MAX_SIZE;
};

#endif // HANDHELDDEVICE_H
