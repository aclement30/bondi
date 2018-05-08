#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
    public:
        virtual void handle() = 0;
        virtual void resume() {
            handle();
        };
        virtual void escape() {};
        virtual void safetyStop() {};
        virtual ~Controller() = default;
};

#endif