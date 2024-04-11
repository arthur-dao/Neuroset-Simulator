#include "HandheldDevice.h"

HandheldDevice::HandheldDevice(int battery) : deviceStatus(ON), runStatus(INACTIVE), batteryPercent(battery), contact(false), treatmentSig(false) {
    // Initialize timers
    connect(&stopTimer, &QTimer::timeout, this, &HandheldDevice::shutdown);
    connect(&runTimer, &QTimer::timeout, this, &HandheldDevice::reduceBattery);
    runTimer.start(3000); // battery lasts 5min. battery is supposed to fully deplete every 2-3 treatments (29s per treatment)
}

HandheldDevice::~HandheldDevice() {}

void HandheldDevice::createSession(){
    if (deviceStatus == OFF || batteryPercent <= 0) {
        qDebug() << "Cannot start session: Device is off or battery is low.";
        return;
    }
    runStatus = ACTIVE;
    // init new session
    qDebug() << "New session created and ready to start.";
    beginSession();
}

void HandheldDevice::shutdown() {
    if (runStatus == ACTIVE || runStatus == PAUSED) {
        stop();
    }
    qDebug() << "Device shutting down...";
    deviceStatus = OFF;
    batteryPercent = 0;
}

void HandheldDevice::beginSession() {
    if (deviceStatus != ON) {
        qDebug() << "Trying to begin a session without it being set to active.";
        return;
    }
    qDebug() << "Session has begun.";
    //start reading eeg data and applying treaments
}

void HandheldDevice::stop() {
    if (runStatus != ACTIVE && runStatus != PAUSED) {
        qDebug() << "No session is currently running or paused to stop.";
        return;
    }
    // do something here
    runStatus = INACTIVE;
    qDebug() << "Session stopped.";
}

void HandheldDevice::pause() {
    if (runStatus != ACTIVE) {
        qDebug() << "No active session to pause.";
        return;
    }
    runStatus = PAUSED;
    qDebug() << "Session paused.";
}

void HandheldDevice::resume() {
    if (deviceStatus == OFF) {
        qDebug() << "Cannot resume: Device is turned off.";
        return;
    } else if (deviceStatus == ON && runStatus == DISCONNECTED) {
        qDebug() << "Cannot resume: Device is disconnected.";
        return;
    }

    if (runStatus == INACTIVE) {
        qDebug() << "No active session found. Starting a new session.";
        createSession();
    }
    else if (runStatus == PAUSED) {
        runStatus = ACTIVE;
        qDebug() << "Session resumed.";
        //resume type shi
    }
    else if (runStatus == ACTIVE) {
        qDebug() << "Session is already active.";
    }
    else {
        qDebug() << "Session cannot be resumed due to unexpected state.";
    }
}

void HandheldDevice::dateTimeSelection() {
    qDebug() << "Date and time selection accessed.";
    // ui component
}

void HandheldDevice::displaySessionLog() {
    qDebug() << "Displaying session log...";
    // iterate session logs
}

void HandheldDevice::navigateUp() {
    qDebug() << "Navigated up in menu.";
}

void HandheldDevice::navigateDown() {
    qDebug() << "Navigated down in menu.";
}

void HandheldDevice::select() {
    qDebug() << "Menu item selected.";
}

void HandheldDevice::menuToggle() {
    qDebug() << "Menu toggled.";
}

void HandheldDevice::uploadToPC() {
    qDebug() << "Session data being prepared for upload to PC...";
}

void HandheldDevice::updateMenu() {
    qDebug() << "Updating menu display...";
}

void HandheldDevice::powerToggle() {
    if (deviceStatus == OFF) {
        deviceStatus = ON;
        qDebug() << "Device powered on. Ready for operation.";
        runStatus = INACTIVE;
        chargeBatteryToFull();
        runTimer.start(3000);
    } else {
        qDebug() << "Shutting down the device...";
        shutdown();
    }
}

void HandheldDevice::reduceBattery() {
    if (batteryPercent > 0) {
        batteryPercent--;
        qDebug() << "Battery level now at " << batteryPercent << "%.";
        if (batteryPercent == 0) {
            shutdown();
        }
    }
}

void HandheldDevice::chargeBatteryToFull() { //link this to some button i guess
    batteryPercent = 100;
    qDebug() << "Battery fully charged.";
}

void HandheldDevice::disconnect() {
    if (deviceStatus == ON && (runStatus == ACTIVE || runStatus == PAUSED)) {
        runStatus = DISCONNECTED;
        qDebug() << "Device disconnected. Awaiting reconnection...";
        // reconnect type shi
    }
}


