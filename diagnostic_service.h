#include "route.h"

#ifndef DIAGNOSTICSERVICE_H
#define DIAGNOSTICSERVICE_H

class DiagnosticService {
    public:
        // DiagnosticService(
        //     RfidReader feederRfidReader,
        //     std::vector<RailPoint> feederRailPoints,
        //     std::vector<Route> feederRoutes
        // ) : 
        //     rfidReader(feederRfidReader),
        //     railPoints(feederRailPoints),
        //     routes(feederRoutes)
        // {}

        void mapRoutes(Feeder &feeder, std::vector<Route> &routes) {
            for (Route &route : routes) {  
                stop();

                if (route.initialDirection == MOVING_FORWARD) {
                    feeder.moveForward();
                } else {
                    moveBackward();
                }
            }
            // std::for_each(routes.begin(), routes.end(),
            //     [](Route * route) {
            //         stop();

            //         if (route.initialDirection == MOVING_FORWARD) {
            //             moveForward();
            //         } else {
            //             moveBackward();
            //         }
            // });
        }
    
        bool isDiagnosticMode() {
            return _diagnosticMode;
        }

    private:
        bool _diagnosticMode = false;
}

#endif