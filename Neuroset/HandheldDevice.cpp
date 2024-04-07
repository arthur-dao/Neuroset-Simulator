#include "HandheldDevice.h"

int HandheldDevice::size = MAX_SIZE;

HandheldDevice::HandheldDevice(int battery){
    batteryPercent = battery;
}

HandheldDevice::~HandheldDevice(){

}

void HandheldDevice::createSession(){
    qInfo("Session started");
}

void HandheldDevice::pauseSession(){

}

void HandheldDevice::shutdown(){
    qInfo("Shutdown");
}

void HandheldDevice::beginSession(){

}

void HandheldDevice::dateTimeSelection(){

}

void HandheldDevice::displaySessionLog(){

}

void HandheldDevice::stop(){
    qInfo("Stop");
}

void HandheldDevice::pause(){
    qInfo("Pause");
}

void HandheldDevice::resume(){
    qInfo("Resume");
}

void HandheldDevice::navigateUp(){

    qInfo("Navigate Up");

}

void HandheldDevice::navigateDown(){

    qInfo("Navigate Down");

}

void HandheldDevice::select(){

}

void HandheldDevice::menuToggle(){
    qInfo("Menu");
}

void HandheldDevice::uploadToPC(){

}

void HandheldDevice::updateMenu(){

}

void HandheldDevice::powerToggle(){
    qInfo("Power");
}

void HandheldDevice::reduceBattery(){
    qInfo("Reduce battery");
    if(batteryPercent != 0){

        batteryPercent--;
        qDebug() << batteryPercent;
    }
}
