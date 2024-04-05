#ifndef HANDHELDDEVICE_H
#define HANDHELDDEVICE_H

#include "defs.h"
#include "Headset.h"
#include "Session.h"

#include "QTimer"

class HandheldDevice{

    public:
        HandheldDevice();
        void createSession();
        void pauseSession();
        void shutdown();
        void beginSession();
        void dateTimeSelection();
        void displaySessionLog();

        void stop();
        void pause();
        void resume();

        void navigate();
        void select();
        void menuToggle();
        void uploadToPC();
        void updateMenu();

        void powerToggle();

    private:
        RunStatus status;
        int batteryPercent;
        bool contact;
        bool treatmentSig;

        QTimer  stopTimer;
        QTimer  runTimer;

        list<Session> sessions;
        static int size;
};

#endif // HANDHELDDEVICE_H
