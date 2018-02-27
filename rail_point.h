#ifndef RAILPOINT_H
#define RAILPOINT_H

class RailPoint {
    public:
        const int id;
        const char *name;

        RailPoint(
            int pointId, 
            const char *pointName
        ) : 
            id(pointId),
            name(pointName)
        {}  
};

#endif