#ifndef CONTROLLERS_CONTROLLER_H
#define CONTROLLERS_CONTROLLER_H

class Controller {
    public:
        virtual void handle() = 0;
        virtual void escape() = 0;
        virtual ~Controller() = default;
};

#endif