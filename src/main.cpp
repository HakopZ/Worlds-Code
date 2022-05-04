#include "main.h"
#include <iomanip>
#include <map>
#include <string>

// Chassis constructor
Drive chassis (
  {-13, 14, 17, 19},    //left motors
  {15, -16, -18, -20},  //right motors 
  11  ,                   //imu port
  2.75,                 //wheel diameter
  600,                  //Motor cartridge
  1                     //Gear Ratio
);

std::map<int, std::string> map_to_name
{
  {13, "Front Left"},
  {14, "Mid Left"},
  {15, "Front Right"},
  {16, "Mid Right"},
  {17, " PTO Left"},
  {18, "PTO Right"},
  {19, "Back Left"},
  {20, "Back Right"},
  
};

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
    Auton("Rush", rush_for_one_goal),
    Auton("Left With Win Point", left_with_win_point),
    Auton("Tall Goal Rush", tall_goal_rush),
    Auton("Right With Win Point", right_with_win_point),
    Auton("Full Winpoint", win_point)
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

void print_motor_temperatures()
{
  for(int i =0; i<chassis.left_motors.size(); i++)
  {
    pros::lcd::print(i+1, "%s: Temp: %.3f", map_to_name[chassis.left_motors[i].get_port()], chassis.left_motors[i].get_temperature());
  }
  for(int i = 0; i< chassis.right_motors.size(); i++)
  {
    pros::lcd::print(4 + i, "%s: Temp: %.3f",  map_to_name[chassis.right_motors[i].get_port()], chassis.right_motors[i].get_temperature());
  }
}
void opcontrol() {
  chassis.set_drive_brake(MOTOR_BRAKE_BRAKE);

  while (true) {
    print_motor_temperatures();
    intake_control();
    chassis.tank();
    
    front_fork.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1));
    back_fork.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2));
    tall_goal_mech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y));
    preload_release.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_X));
    kick_stand.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP));
     
    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
