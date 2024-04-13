#ifndef TREATMENT_H
#define TREATMENT_H

#include <QObject>
#include "Headset.h"

class Treatment : public QObject {
    Q_OBJECT

    public:
        Treatment();


    public slots:
        void beginTreatment(Headset* headset);

};

#endif // TREATMENT_H
