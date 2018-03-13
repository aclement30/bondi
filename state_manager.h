#ifndef STATEMANAGER_H
#define STATEMANAGER_H

using namespace std;

class DirectionAware {
    public:
        virtual void didChangeDirection(MovingDirection movingDirection) = 0;
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
            Serial.print("New state:");
            Serial.println(newState);

            if (currentState == newState) {
                return;
            }

            previousState = currentState;
            currentState = newState;
            
            switch(currentState) {
                case Off: {
                    Serial.println("MODE: ARRET");
                    break;
                }
                case MainMenu: {
                    Serial.println("MENU PRINCIPAL");
                    break;
                }
                case Automatic: {
                    Serial.println("MODE: AUTO");
                    break;
                }
                case Manual: {
                    Serial.println("MODE: MANUEL");
                    break;
                }
                case Diagnostic: {
                    Serial.println("MODE: DIAGNOSTIC");
                    break;
                }
            }
        }

        MovingDirection getMovingDirection() {
            return movingDirection;
        }

        void changeMovingDirection(MovingDirection newDirection) {
            if (movingDirection == newDirection) return;

            movingDirection = newDirection;
            notifyDirectionObservers(newDirection);
        }

        void notifyDirectionObservers(MovingDirection direction) {
            for (int i = 0; i < directionObservers.size(); i++) {
                directionObservers[i]->didChangeDirection(direction);
            }
        }

        void subscribeToDirection(DirectionAware *observer) {
            directionObservers.push_back(observer);
        }

        void stop() {
            changeMovingDirection(MOVING_IDLE);
        }

        bool isSafetyMode() {
            return safetyModeActivated;
        }

        void safetyStop() {
            safetyModeActivated = true;
            stop();
        }

    private:
        MachineState currentState = Off;
        MachineState previousState = Off;
        MovingDirection movingDirection = MOVING_IDLE;
        bool safetyModeActivated = false;
        vector <class DirectionAware *> directionObservers;

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