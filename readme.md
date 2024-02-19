# TactiDriver
Step Dir based control for stepper motors using TactiCom communication protocol.

## Pinout
| Pin | Function     |
|-----|--------------|
| 2   | Motor 0 Step |
| 3   | Motor 0 Dir  |
| 4   | Motor 1 Step |
| 5   | Motor 1 Dir  |
| 6   | Motor 2 Step |
| 7   | Motor 2 Dir  |
| 8   | Motor 3 Step |
| 9   | Motor 3 Dir  |

Edit `NB_STEPPERS` constant & `steppers` array to add or remove steppers.

## Usage
Commands are sent over serial over the TactiCom protocol using prefix `TD`. The following commands are supported:
- `cs=<speed_0>`: Run all steppers at the same speed.
- `cs=<speed_0>,<speed_1>,<speed_2>,<speed_3>`: Run all steppers at the specified speeds.
- `cs_i=<stepper_id>,<speed>`: Run a specific stepper at a specific speed (id starts from 0).
- `go=<steps_0>,<speed_0>`: Move all steppers by the same number of steps at the specified speed.
- `go=<steps_0>,<speed_0>,<steps_1>,<speed_1>,<steps_2>,<speed_2>,<steps_3>,<speed_3>`: Move all steppers by the specified number of steps at the specified speeds.
- `go_i=<stepper_id>,<steps>,<speed>`: Move a specific stepper by the specified number of steps at the specified speed.