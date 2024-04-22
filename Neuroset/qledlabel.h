#include <QLabel>
#include <QTimer>

/**
 * @file QLedLabel.h
 * @brief The QLedLabel class represents a label with LED-like functionality.
 * 
 * This class inherits from QLabel and provides functionality to display LED-like states
 * such as black, initiated blue, contact lost red, and treatment green.
 */

class QLedLabel : public QLabel {
    Q_OBJECT
public:
    explicit QLedLabel(QWidget *parent = nullptr);
    ~QLedLabel();

    enum State {
        StateBlack,
        StateInitiatedBlue,
        StateContactLostRed,
        StateTreatmentGreen
    };

    void startFlashing(int interval = 500);
    void stopFlashing();

public slots:
    void setGreen();
    void setBlack();
    void setBlue();
    void setRed();
    void setState(State state);

private slots:
    void toggleVisibility();

private:
    QTimer *flashTimer;
    State currentState;
    bool isFlashing;
};
