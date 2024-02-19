#include <Arduino.h>
#include <tacticom.h>
#include <AccelStepper.h>

#define NB_STEPPERS 4

AccelStepper steppers[NB_STEPPERS] = {
    AccelStepper(AccelStepper::DRIVER, 2, 3),
    AccelStepper(AccelStepper::DRIVER, 4, 5),
    AccelStepper(AccelStepper::DRIVER, 6, 7),
    AccelStepper(AccelStepper::DRIVER, 8, 9)
};


enum { SPEED = 0, POSITION = 1 } mode = POSITION;

bool prev_any_running = false; // Used to detect when a move is done

void commands_handler(const String &name, const String &answer_code, const String &ask_code, const String *args,
                      uint8_t args_count);

TactiCom tc("TD", commands_handler);


void setup() {
    Serial.begin(115200);

    for (auto &stepper: steppers) {
        stepper.setMaxSpeed(1000);
    }
}

void loop() {
    tc.tick();

    bool any_running = false;
    for (auto &stepper: steppers) {
        if (mode == SPEED || stepper.distanceToGo() != 0) {
            stepper.runSpeed();
            any_running = true;
        }
    }

    if (!any_running && any_running != prev_any_running) {
        tc.send_event("move_done");
    }
    prev_any_running = any_running;
}

void commands_handler(const String &name, const String &answer_code, const String &ask_code, const String *args,
                      const uint8_t args_count) {
    if (name == "cs") {
        if (args_count != NB_STEPPERS && args_count != 1) {
            const String error = "missing_args";
            tc.send_reply("cs_error", ask_code, &error, 1);
            return;
        }

        mode = SPEED;

        for (uint8_t i = 0; i < NB_STEPPERS; i++) {
            const float speed = args_count == 1 ? args[0].toFloat() : args[i].toFloat();
            steppers[i].setSpeed(speed);
        }
    } else if (name == "cs_i") {
        if (args_count != 2) {
            const String error = "missing_args";
            tc.send_reply("cs_i_error", ask_code, &error, 1);
            return;
        }

        mode = SPEED;
        steppers[args[0].toInt()].setSpeed(args[1].toFloat());
    } else if (name == "go") {
        if (args_count != NB_STEPPERS * 2 && args_count != 2) {
            const String error = "missing_args";
            tc.send_reply("go_error", ask_code, &error, 1);
            return;
        }

        mode = POSITION;

        for (uint8_t i = 0; i < NB_STEPPERS; i++) {
            const long move = args_count == 2 ? args[0].toInt() : args[i * 2].toInt();
            const float speed = args_count == 2 ? args[1].toFloat() : args[i * 2 + 1].toFloat();

            steppers[i].move(move);
            steppers[i].setSpeed(speed * (steppers[i].distanceToGo() > 0 ? 1 : -1));
        }
    } else if (name == "go_i") {
        if (args_count != 3) {
            const String error = "missing_args";
            tc.send_reply("go_i_error", ask_code, &error, 1);
            return;
        }

        mode = POSITION;

        const long i = args[0].toInt();
        const long move = args[1].toInt();
        const float speed = args[2].toFloat();

        steppers[i].move(move);
        steppers[i].setSpeed(speed * (steppers[i].distanceToGo() > 0 ? 1 : -1));
    }
}
