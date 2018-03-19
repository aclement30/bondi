#include "display_service.h"
#include "navigation_menu.h"

#ifndef DISPLAYSCREENS_H
#define DISPLAYSCREENS_H

struct Meal {
    const char name[11];
};

struct DisplayScreens {

    // MAIN SCREENS

    static void startup() {
        DisplayService::getInstance().clearScreen();
        DisplayService::getInstance().printCenter("Demarrage en cours", 3);
    }

    // static void mainMenu() {
    //     vector<string> menuOptions = {
    //         "Mode AUTO.",
    //         "Mode MANUEL",
    //         "Historique",
    //         "Diagnostic",
    //         "Redemarrage"
    //     };

    //     NavigationMenu menu = NavigationMenu("MENU PRINCIPAL", menuOptions, "");
    //     menu.show();
    //     int selectedOption = menu.waitForSelection();

    //     switch(selectedOption) {
    //         case 1:
    //             DisplayScreens::automaticMode();
    //             break;
    //         case 2:
    //             DisplayScreens::manualMode();
    //             break;
    //         case 4:
    //             DisplayScreens::diagnostic();
    //             break;
    //         case 5:
    //             asm volatile ("  jmp 0");
    //             break;
    //     }
    // }

    // // AUTOMATIC MODE SCREENS

    // static void automaticMode() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("MODE: AUTO");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("En attente", 2);

    //     delay(2000);

    //     DisplayScreens::mealDistribution();
    // }

    // static void mealDistribution() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("DISTRIBUTION AUTO.");
    //     DisplayService::getInstance().print(" Repas: Mat-G.Etb", 1);
    //     DisplayService::getInstance().print("Groupe: G-1", 2);
    //     DisplayService::getInstance().print("  Alim: 500 | 600", 3);

    //     delay(4000);
    // }

    // // MANUAL MODE SCREENS

    // static void manualMode() {
    //     vector<string> menuOptions = {
    //         "Selection repas",
    //         "Controle manuel"
    //     };

    //     NavigationMenu menu = NavigationMenu("MODE: MANUEL", menuOptions);
    //     menu.show();
    //     int selectedOption = menu.waitForSelection();

    //     switch(selectedOption) {
    //         case 1:
    //             DisplayScreens::mealSelection();
    //             break;
    //         case 2:
    //             DisplayScreens::manualControl();
    //             break;
    //         case -1:
    //             DisplayScreens::mainMenu();
    //             break;
    //     }
    // }

    // static void mealSelection() {
    //     Meal meal1 = { "Mat-G.Etb" };
    //     Meal meal2 = { "Midi-P.Etb" };

    //     vector<Meal> mealNames = {
    //         meal1,
    //         meal2
    //     };
        
    //     vector<string> menuOptions;
    //     for(int n = 0; n < mealNames.size(); n++) {
    //         char mealName[15];
    //         strcpy(mealName, mealNames[n].name);
    //         strncat(mealName, "              ", 14 - strlen(mealName));
    //         mealName[13] = '*';
    //         Serial.println(mealName);
    //         menuOptions.push_back(string(mealName));
    //     }

    //     NavigationMenu menu = NavigationMenu("SELECTION REPAS", menuOptions);
    //     menu.show();
    //     int selectedOption = menu.waitForSelection();

    //     switch(selectedOption) {
    //         case 1:
    //             DisplayScreens::mealDistribution();
    //             break;
    //         case 2:
    //             DisplayScreens::mealDistribution();
    //             break;
    //         case -1:
    //             DisplayScreens::manualMode();
    //             break;
    //     }
    // }

    // static void manualControl() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("CONTROLE MANUEL");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("En attente", 2);

    //     delay(3000);
    // }

    // static void diagnostic() {
    //     vector<string> menuOptions = {
    //         "Liste routes",
    //         "Test mot. rail",
    //         "Test conv. av",
    //         "Test conv. arr"
    //     };

    //     NavigationMenu menu = NavigationMenu("DIAGNOSTIC", menuOptions);
    //     menu.show();
    //     int selectedOption = menu.waitForSelection();

    //     switch(selectedOption) {
    //         case 1:
    //             DisplayScreens::diagnosticRouteMapping();
    //             break;
    //         case 2:
    //             DisplayScreens::diagnosticRailMotor();
    //             break;
    //         case 3:
    //             DisplayScreens::diagnosticConveyorMotor();
    //             break;
    //         case 4:
    //             DisplayScreens::diagnosticConveyorMotor();
    //             break;
    //         case -1:
    //             DisplayScreens::mainMenu();
    //             break;
    //     }
    // }

    // static void diagnosticRouteMapping() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("DIAGNO. LISTE ROUTES");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("Commencer [F1]", 2);

    //     bool canStart = DisplayService::waitForConfirmation();

    //     if (canStart) {
    //         DisplayScreens::diagnosticRouteMappingDetail();
    //     } else {
    //         DisplayScreens::diagnostic();
    //     }
    // }

    // static void diagnosticRouteMappingDetail() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("DIAGNO. LISTE ROUTES");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().print("Route: GE", 2);
    //     DisplayService::getInstance().print("Point: G-2", 3);

    //     delay(2000);

    //     DisplayScreens::diagnosticRouteMappingCompleted();
    // }

    // static void diagnosticRouteMappingCompleted() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("DIAGNO. LISTE ROUTES");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("Recommencer [F1]", 2);
    //     DisplayService::getInstance().printCenter("Quitter [F4]", 3);

    //     bool restart = DisplayService::waitForConfirmation();

    //     if (restart) {
    //         DisplayScreens::diagnosticRouteMappingDetail();
    //     } else {
    //         DisplayScreens::diagnostic();
    //     }
    // }

    // static void diagnosticRailMotor() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("TEST MOTEUR RAIL");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("Commencer [F1]", 2);

    //     bool canStart = DisplayService::waitForConfirmation();

    //     if (canStart) {
    //         DisplayScreens::diagnosticRailMotorDetail();
    //     } else {
    //         DisplayScreens::diagnostic();
    //     }
    // }

    // static void diagnosticRailMotorDetail() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("TEST MOTEUR RAIL");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter(">>> AVANT >>>", 2);

    //     delay(2000);

    //     DisplayService::getInstance().printCenter("<<< ARRIERE <<<", 2);

    //     delay(2000);

    //     DisplayScreens::diagnosticRailMotorCompleted();
    // }

    // static void diagnosticRailMotorCompleted() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("TEST MOTEUR RAIL");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("Recommencer [F1]", 2);
    //     DisplayService::getInstance().printCenter("Quitter [F4]", 3);

    //     bool restart = DisplayService::waitForConfirmation();

    //     if (restart) {
    //         DisplayScreens::diagnosticRailMotorDetail();
    //     } else {
    //         DisplayScreens::diagnostic();
    //     }
    // }

    // static void diagnosticConveyorMotor() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("TEST CONVOYEUR AVANT");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("Commencer [F1]", 2);

    //     bool canStart = DisplayService::waitForConfirmation();

    //     if (canStart) {
    //         DisplayScreens::diagnosticConveyorMotorDetail();
    //     } else {
    //         DisplayScreens::diagnostic();
    //     }
    // }

    // static void diagnosticConveyorMotorDetail() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("TEST CONVOYEUR AVANT");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter(">>> DROITE >>>", 2);
    //     DisplayService::getInstance().printCenter("500 rpm", 3);

    //     delay(2000);

    //     DisplayService::getInstance().printCenter("<<< GAUCHE <<<", 2);
    //     DisplayService::getInstance().printCenter("200 rpm", 3);

    //     delay(2000);

    //     DisplayScreens::diagnosticConveyorMotorCompleted();
    // }

    // static void diagnosticConveyorMotorCompleted() {
    //     DisplayService::getInstance().clearScreen();
    //     DisplayService::getInstance().printTitle("TEST CONVOYEUR AVANT");
    //     DisplayService::getInstance().addBorder();
    //     DisplayService::getInstance().printCenter("Recommencer [F1]", 2);
    //     DisplayService::getInstance().printCenter("Quitter [F4]", 3);

    //     bool restart = DisplayService::waitForConfirmation();

    //     if (restart) {
    //         DisplayScreens::diagnosticConveyorMotorDetail();
    //     } else {
    //         DisplayScreens::diagnostic();
    //     }
    // }
};

#endif
