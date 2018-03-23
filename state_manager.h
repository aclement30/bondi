#include "constants.h"

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

class StateManager {
    public:
        static StateManager& getInstance() {
            static StateManager instance;   // Guaranteed to be destroyed.
                                            // Instantiated on first use.
            return instance;
        }

        MachineState getState();
        MachineState getPreviousState();
        void changeState(MachineState newState);
        MovingDirection getMovingDirection();
        void changeMovingDirection(MovingDirection newDirection);
        void stop();
        bool isSafetyMode();
        void safetyStop();
        void disengageSafetyMode();

    private:
        MachineState currentState;
        MachineState previousState;
        MovingDirection movingDirection;
        bool safetyModeActivated;

        StateManager();

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        // DisplayService(DisplayService const&);   // Don't Implement
        void operator = (StateManager const&); // Don't implement
};

#endif