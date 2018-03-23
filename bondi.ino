#include <ArduinoSTL.h>
//#include <avr/wdt.h>

#include "config.h"
#include "constants.h"
#include "state_manager.h"

// Controllers
#include "controller.h"
#include "automatic_controller.h"
#include "diagnostic_controller.h"
#include "history_controller.h"
#include "main_menu_controller.h"
#include "manual_control_controller.h"
#include "manual_meal_distribution_controller.h"
#include "manual_menu_controller.h"
#include "off_controller.h"

// Motors
#include "conveyor_motor.h"
#include "rail_motor.h"

// Services
#include "keypad_service.h"
#include "location_service.h"
#include "safety_service.h"

// MOTORS

RailMotor mainMotor = RailMotor();
ConveyorMotor conveyorFront = ConveyorMotor(
    CONVEYOR_MOTOR_FRONT_PWM,
    CONVEYOR_MOTOR_FRONT_REVERSE
);
ConveyorMotor conveyorBack = ConveyorMotor(
    CONVEYOR_MOTOR_BACK_PWM,
    CONVEYOR_MOTOR_BACK_REVERSE
);

// Load config
Config config = loadStaticConfiguration();
Config sdConfig = loadSDCardConfiguration();

// Services

LocationService locationService = LocationService(config.railPoints, config.routes);
SafetyService safetyService = SafetyService();
MealService mealService = MealService(conveyorFront, conveyorBack, config.meals, locationService);

Controller *currentControllerPtr = NULL;
MachineState previousState = Off;

bool isPowerON() {
    return digitalRead(POWER_BUTTON) == LOW;
}

void createController(MachineState currentState) {
    if (currentControllerPtr != NULL) {
        if (previousState == currentState) {
        //   Serial.println(" (use existing controller)");
          return;
        }

        delete currentControllerPtr;
        currentControllerPtr = NULL;
    }

    previousState = currentState;

    switch(currentState) {
        case Off: {
            currentControllerPtr = new OffController();
            break;
        }
        case MainMenu: {
            currentControllerPtr = new MainMenuController();
            // Serial.println(" (new MainMenuController)");
            break;
        }
        case Automatic: {
            currentControllerPtr = new AutomaticController(mealService);
            // Serial.println(" (new AutomaticController)");
            break;
        }
        case ManualMenu: {
            currentControllerPtr = new ManualMenuController();
            // Serial.println(" (new ManualMenuController)");
            break;
        }
        case ManualMealDistribution: {
            currentControllerPtr = new ManualMealDistributionController(mealService);
            // Serial.println(" (new ManualControlController)");
            break;
        }
        case ManualControl: {
            currentControllerPtr = new ManualControlController(mealService);
            // Serial.println(" (new ManualControlController)");
            break;
        }
        case History: {
            currentControllerPtr = new HistoryController(mealService);
            // Serial.println(" (new HistoryController)");
            break;
        }
        case Diagnostic: {
            currentControllerPtr = new DiagnosticController(
                locationService,
                mainMotor,
                conveyorFront,
                conveyorBack
            );
            // Serial.println(" (new DiagnosticController)");
            break;
        }
    }
}

void displayStartupScreen() {
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printCenter("Demarrage en cours", 3);
}

void setup() {
    Serial.begin(9600);   // open serial over USB
    Serial.println("Démarrage en cours");

    //wdt_enable(WDTO_1S);
  
    displayStartupScreen();

    for (int n = 0; n < INPUTS_COUNT; n++) {
        pinMode(INPUTS[n], INPUT);
    }

    //if (!SD.begin(SD_CARD_CS)) {
    //    Serial.println("Card failed, or not present");
    //    // don't do anything more:
    //    while (1);
    //}

    Serial.println("Configuration initiale");

    StateManager::getInstance().changeState(MainMenu);
    KeypadService::getInstance().listenForEscape();
}

void loop() {
    //wdt_reset();
  
    Serial.println("* loop");
    
    // Stop right here if power is OFF
    if (!isPowerON()) {
        delay(1000);
        return;
    }

    // Serial.println("* check safety");
    safetyService.checkSafetyState();
    
    Serial.println("* refresh point");
    locationService.refreshActivePoint();
    
    // Serial.println("* main motor");
    mainMotor.loop();
    
    // Serial.print("* create controller");
    createController(StateManager::getInstance().getState());
    
    Serial.println("* handle");

    delay(250);

    if (KeypadService::getInstance().isEscapeKeyPressed()) {
        // Stop motor
        mainMotor.stop();

        currentControllerPtr->escape();
        
        KeypadService::getInstance().resetEscapeKey();
    } else {
        currentControllerPtr->handle();
    }
}
