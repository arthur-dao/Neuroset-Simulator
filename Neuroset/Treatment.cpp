#include "Treatment.h"

Treatment::Treatment(){

}

void Treatment::beginTreatment(Headset* headset){

    headset->startSimulation(500);

}
