#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>

class QLedLabel : public QLabel {
    Q_OBJECT
public:
    explicit QLedLabel(QWidget *parent = 0);

    enum State {
        StateOkOrange,
        StateInitiatedBlue,
        StateContactLostRed,
        StateTreatmentGreen
    };

signals:
public slots:
    void setState(State state);
    void setState(bool state);
};

#endif // QLEDLABEL_H
