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
                    Serial.println("MODE: arrêt");
                    break;
                case Idle:
                    Serial.println("MODE: inactif");
                    break;
                case Diagnostic:
                    Serial.println("MODE: diagnostic");
                    break;
                case Manual:
                    Serial.println("MODE: manuel");
                    break;
                case Automatic:
                    Serial.println("MODE: automatique");
                    break;
            }
        }

    private:
        MachineState currentState = Off;
        MachineState previousState = Off;
};

#endif