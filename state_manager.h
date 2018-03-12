#include "controllers/controller.h"
#include "controllers/automatic_controller.h"
#include "controllers/diagnostic_controller.h"
#include "controllers/main_menu_controller.h"
#include "controllers/off_controller.h"

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

        MachineState getState() {
            return currentState;
        }

        MachineState getPreviousState() {
            return previousState;
        }

        void changeState(MachineState newState) {
            if (previousState == currentState) return;

            previousState = currentState;
            currentState = newState;

            switch(currentState) {
                case Off:
                    Serial.println("MODE: ARRET");
                    Controller currentController = OffController();
                    break;
                case MainMenu:
                    Serial.println("MENU PRINCIPAL");
                    Controller currentController = MainMenuController();
                    break;
                case Automatic:
                    Serial.println("MODE: AUTO");
                    Controller currentController = AutomaticController();
                    break;
                case Manual:
                    Serial.println("MODE: MANUEL");
                    break;
                case Diagnostic:
                    Serial.println("MODE: DIAGNOSTIC");
                    Controller currentController = DiagnosticController();
                    break;
            }
        }

        Controller& getController() {
            return &currentController;
        }

        void changeMovingDirection(MovingDirection newDirection) {
            if (movingDirection == newDirection) return;

            movingDirection = newDirection;
        }

    private:
        MachineState currentState = Off;
        MachineState previousState = Off;
        MovingDirection movingDirection = MOVING_IDLE;
        bool safetyStop = false;
        Controller currentController = OffController();

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