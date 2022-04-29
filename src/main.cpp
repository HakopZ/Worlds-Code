#include "main.h"

// Chassis constructor
Drive chassis (
  {-13, 14, 17, 19},    //left motors
  {15, -16, -18, -20},  //right motors 
  11  ,                   //imu port
  2.75,                 //wheel diameter
  600,                  //Motor cartridge
  1                     //Gear Ratio
);



void initialize() {
  // Print our branding over your terminal :D
  ez::print_ez_template();
  pros::delay(500);
  anti_jam_task.suspend();
  chassis.toggle_modify_curve_with_controller(true); 
  chassis.set_active_brake(0); 
  chassis.set_curve_default(0, 0);   
  default_constants();
  exit_condition_defaults();
  
  chassis.set_left_curve_buttons(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_A);

  ez::as::auton_selector.add_autons
  ({
    Auton("David Auto", david_auto), 
    Auton("Tall Goal Rush", tall_goal_rush),
    Auton("Left With Win Point", left_with_win_point),
    Auton("Rush", right_with_win_point),
    Auton("Full Winpoint", win_point),
    Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example)
  });

  
  chassis.initialize();
  ez::as::initialize();
}

void disabled() { 
}

void competition_initialize() {
  
}

void autonomous() {
  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
  chassis.reset_drive_sensor(); // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.
  ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.
}

void opcontrol() {
  chassis.set_drive_brake(MOTOR_BRAKE_BRAKE);

  while (true) {

    intake_control();
    chassis.tank();
    
    front_fork.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1));
    back_fork.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2));
    tall_goal_mech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y));
    preload_release.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_X));

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
