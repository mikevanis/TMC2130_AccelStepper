#define EN_PIN 3
#define DIR_PIN 4
#define STEP_PIN 5
#define CS_PIN 6
#define LED 8

boolean isClockwise = false;

#include <TMC2130Stepper.h>
TMC2130Stepper driver = TMC2130Stepper(EN_PIN, DIR_PIN, STEP_PIN, CS_PIN);

#include <AccelStepper.h>
AccelStepper stepper = AccelStepper(stepper.DRIVER, STEP_PIN, DIR_PIN);

#include <SPI.h>

// Microstepping - 0, 2, 4, 8, 16, 32, 64, 128, 255. The lower the value, the faster the motor.
byte microstepsVal = 4;

unsigned long prevMillis;

void setup() {
  pinMode(LED, OUTPUT);
  for(int i=0; i<4; i++) {
    digitalWrite(LED, HIGH);
    delay(10);
    digitalWrite(LED, LOW);
    delay(100);
  }

  SPI.begin();
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Start...");
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  driver.begin();
  driver.rms_current(800); // RMS current - increase to 800 or 1000 to get more torque. Although you'll need to put a heatsink on the chip.
  driver.stealthChop(1); // Enable extremely quiet stepping.
  driver.stealth_autoscale(1);
//  driver.SilentStepStick2130(800); // Set stepper current to 600mA
//  driver.stealthChop(1); // Enable extremely quiet stepping
  driver.microsteps(microstepsVal);

  stepper.setMaxSpeed(2000); // Max speed in steps / second. Do not set higher than 2000.
  stepper.setAcceleration(500); // Purely aesthetic, the lower the acceleration value, the slower the ramp at start / end.
  stepper.setEnablePin(EN_PIN);
  stepper.setPinsInverted(false, false, true);
  stepper.enableOutputs();
}

void loop() {
  if (stepper.distanceToGo() == 0) {
    digitalWrite(LED, HIGH);
    delay(10);
    digitalWrite(LED, LOW);
    //stepper.disableOutputs();
    delay(100);
    if (isClockwise) {
      stepper.move(9223372036854775806); // Move 400 steps.
      isClockwise = false;
    }
    else {
      stepper.move(-54000); // Move 400 steps in the opposite direction.
      isClockwise = true;
    }
    stepper.enableOutputs();
  }
  
  stepper.run();
}
