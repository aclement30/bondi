#ifndef STATEMANAGER_H
#define STATEMANAGER_H

using namespace std;

enum MachineState {
    Off,
    MainMenu,
    Automatic,
    Manual,
    Diagnostic
};

class StateManager {
    public:
        static StateManager& getInstance() {
            static StateManager instance;   // Guaranteed to be destroyed.
                                            // Instantiated on first use.
            return instance;
        }

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
                case MainMenu:
                    Serial.println("MENU PRINCIPAL");
                    break;
                case Automatic:
                    Serial.println("MODE: AUTO");
                    break;
                case Manual:
                    Serial.println("MODE: MANUEL");
                    break;
                case Diagnostic:
                    Serial.println("MODE: DIAGNOSTIC");
                    break;
            }
        }

    private:
        MachineState currentState = Off;
        MachineState previousState = Off;

        StateManager() {}

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        // DisplayService(DisplayService const&);   // Don't Implement
        void operator = (StateManager const&); // Don't implement

};

#endif