// #ifndef __EMSCRIPTEN__
// #include "ArduinoSTL.h"
// #endif

// #include "rail_point.h"
// #include "route.h"

// #ifndef MEAL_H
// #define MEAL_H

// class MealSequence {
//     public:
//         string name;
//         RailPoint startPoint;
//         RailPoint endPoint;
//         int feed1Flow;
//         int feed2Flow;

//         MealSequence(
//             string sequenceName, 
//             RailPoint sequenceStartPoint, 
//             RailPoint sequenceEndPoint, 
//             int sequenceFeed1Flow, 
//             int sequenceFeed2Flow
//         ) : 
//             name(sequenceName), 
//             startPoint(sequenceStartPoint),
//             endPoint(sequenceEndPoint),
//             feed1Flow(sequenceFeed1Flow),
//             feed2Flow(sequenceFeed2Flow)
//         {}
// };

// class Meal {
//     public:
//         int id;
//         string name;
//         int startMoment;
//         Route route;
//         std::vector<MealSequence> sequences;

//         Meal(
//             int mealId,
//             string mealName,
//             int mealStartMoment, 
//             Route mealRoute,
//             std::vector<MealSequence> mealSequences
//         ) : 
//             id(mealId),
//             name(mealName),
//             startMoment(mealStartMoment), 
//             route(mealRoute),
//             sequences(mealSequences)
//         {}

//         MealSequence* getMealSequenceAt(RailPoint currentRailPoint) {
//             for(int n = 0; n < sequencesCount; n++) {
//                 const MealSequence sequence = sequences[n];
//                 if (sequence.startPoint.id == currentRailPoint.id) {
//                     return &sequences[n];
//                 }
//             }

//             return NULL;
//         }
// };

// Meal getMealById(std::vector<Meal> meals, int mealId) {
//     std::vector<Meal>::iterator iterator = std::find_if (meals.begin(), meals.end(), [&](const Meal & meal) {
//         return meal.id == mealId;
//     });

//     return *iterator;
// }

// #endif