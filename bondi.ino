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

#include "string.h"

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

Config config = loadStaticConfiguration();

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
            // Serial.println(F(" (new MainMenuController)"));
            break;
        }
        case Automatic: {
            currentControllerPtr = new AutomaticController(mealService);
            // Serial.println(F(" (new AutomaticController)"));
            break;
        }
        case ManualMenu: {
            currentControllerPtr = new ManualMenuController();
            // Serial.println(F(" (new ManualMenuController)"));
            break;
        }
        case ManualMealDistribution: {
            currentControllerPtr = new ManualMealDistributionController(mealService);
            // Serial.println(F(" (new ManualControlController)"));
            break;
        }
        case ManualControl: {
            currentControllerPtr = new ManualControlController(mealService);
            // Serial.println(F(" (new ManualControlController)"));
            break;
        }
        case History: {
            currentControllerPtr = new HistoryController(mealService);
            // Serial.println(F(" (new HistoryController)"));
            break;
        }
        case Diagnostic: {
            currentControllerPtr = new DiagnosticController(
                locationService,
                mainMotor,
                conveyorFront,
                conveyorBack
            );
            // Serial.println(F(" (new DiagnosticController)"));
            break;
        }
    }
}

void displayStartupScreen() {
    const static char startupString[] PROGMEM = "Demarrage en cours";
    DisplayService::getInstance().clearScreen();
    DisplayService::getInstance().printCenter(getString(startupString), 3);
}

void setup() {
    Serial.begin(9600);   // open serial over USB
    Serial.println(F("Démarrage en cours"));

    // Init SPI bus
    SPI.begin();

    //wdt_enable(WDTO_1S);
  
    displayStartupScreen();

    // Load config
    //config = loadSDCardConfiguration();

    for (int n = 0; n < INPUTS_COUNT; n++) {
        pinMode(INPUTS[n], INPUT);
    }

    locationService.setup();

    Serial.println(F("Configuration initiale"));

    StateManager::getInstance().changeState(MainMenu);
    KeypadService::getInstance().listenForEscape();
}

void loop() {
    //wdt_reset();
  
    Serial.println(F("* loop"));
    
    // Stop right here if power is OFF
    if (!isPowerON()) {
        delay(1000);
        return;
    }

    // Serial.println("* check safety");
    safetyService.checkSafetyState();
    
    Serial.println(F("* refresh point"));
    locationService.refreshActivePoint();
    
    // Serial.println("* main motor");
    mainMotor.loop();
    
    // Serial.print("* create controller");
    createController(StateManager::getInstance().getState());
    
    Serial.println(F("* handle"));

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
