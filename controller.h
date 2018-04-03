#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
    public:
        virtual void handle() = 0;
        virtual void resume() = 0;
        virtual void escape() = 0;
        virtual ~Controller() = default;
};

#endif