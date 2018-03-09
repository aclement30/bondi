#ifndef STATEMANAGER_H
#define STATEMANAGER_H

using namespace std;

enum MachineState {
    Off,
    Idle,
    Diagnostic,
    Manual,
    Automatic
};

class StateManager {
    public:
        MachineState getState() {
            return currentState;
        }

        MachineState getPreviousState() {
            return previousState;
        }

        void changeState(MachineState newState) {
            previousState = currentState;
            currentState = newState;

            switch(currentState) {
                case Off:
                    Serial.println("MODE: ARRET");
                    break;
                case Idle:
                    Serial.println("MODE: INACTIF");
                    break;
                case Diagnostic:
                    Serial.println("MODE: DIAGNOSTIC");
                    break;
                case Manual:
                    Serial.println("MODE: MANUEL");
                    break;
                case Automatic:
                    Serial.println("MODE: AUTO");
                    break;
            }
        }

    private:
        MachineState currentState = Off;
        MachineState previousState = Off;
};

#endif