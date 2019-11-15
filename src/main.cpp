#include "285z/initRobot.hpp"
#include "285z/functions.hpp"
#include "285Z_Subsystems/tray.hpp"
#include "../include/285Z_Aux/gui.hpp"
 /** Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  initScreen();
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {

  std::shared_ptr<okapi::OdomChassisController> chassis = okapi::ChassisControllerBuilder()
                    .withMotors({ 11, 12 }, { -13, -10 })
                    .withGearset(okapi::AbstractMotor::gearset::green)
                    .withDimensions(scales)
                    .withMaxVelocity(120)
                    .withGains(
                 {0.001, 0.001, 0.00009}, // Distance controller gains
                 {0.001, 0.001, 0.00001}, // Turn controller gains
                 {0.001, 0.001, 0.0001}  // Angle controller gains (helps drive straight)
             )
                    .withOdometry(okapi::StateMode::FRAME_TRANSFORMATION, 0_mm, 0_deg, 0.00001_mps)
                    .buildOdometry();
  std::shared_ptr<okapi::ChassisModel> model = std::dynamic_pointer_cast<okapi::ChassisModel>(chassis->getModel());

  blueLongGoal(chassis);

}


void opcontrol() {
  std::shared_ptr<okapi::OdomChassisController> chassis = okapi::ChassisControllerBuilder()
										.withMotors({ 11, 12 }, { -13, -10 })
										.withGearset(okapi::AbstractMotor::gearset::green)
										.withDimensions(scales)
										.withOdometry(okapi::StateMode::FRAME_TRANSFORMATION, 0_mm, 0_deg, 0.0001_mps)
										.buildOdometry();
	std::shared_ptr<okapi::ChassisModel> model = std::dynamic_pointer_cast<okapi::ChassisModel>(chassis->getModel());

  while(true){

    // TANK DRIVE CODE //
    model->tank(controller.getAnalog(okapi::ControllerAnalog::leftY),
          controller.getAnalog(okapi::ControllerAnalog::rightY));

    //  INTAKE TOGGLE CODE  //
    toggleIntake();
    intakeRev();

    //  ANGLER TOGGLE CODE  //
    anglerToggle();
    stopPID();

    //  LIFT  //
    liftToggle();

    //liftPID();
    pros::delay(10);
  }
}
