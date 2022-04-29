#include "autons.hpp"
#include "main.h"

const int DRIVE_SPEED = 110;
const int TURN_SPEED  = 90;
const int SWING_SPEED = 90;




void default_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void tuning_constants() {
  chassis.set_slew_min_power(70, 70);
  chassis.set_slew_distance(10, 10);
  chassis.set_pid_constants(&chassis.headingPID, 10, 0, 20, 0);   //12 15
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.825, 0, 6, 0);  //0.5  6
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.825, 0, 6, 0);
  chassis.set_pid_constants(&chassis.turnPID, 2.73, 0, 26, 0);
  chassis.set_pid_constants(&chassis.swingPID, 4.9, 0, 50, 0);
}

void front_mogo_constants() {
  chassis.set_slew_min_power(70, 70);
  chassis.set_slew_distance(10, 10);
  chassis.set_pid_constants(&chassis.headingPID, 10, 0, 20, 0);   //12 15
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.825, 0, 6, 0);  //0.5  6
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.825, 0, 6, 0);
  chassis.set_pid_constants(&chassis.turnPID, 2.7, 0, 32, 0);
  chassis.set_pid_constants(&chassis.swingPID, 4.9, 0, 50, 0);
}

void two_mogo_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void exit_condition_defaults() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}


bool tug_in_case(int distance) {
  while (chassis.interfered) {
    // Attempt to drive backwards
    chassis.set_drive_pid(distance, 127);
    chassis.wait_drive();
    pros::lcd::set_text(6, "Tugging");
    // If failsafed...
    if (chassis.interfered) {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(ez::util::sgn(distance) * 2, 20);
      pros::delay(1000);
    }
    // If robot successfully drove back, return
    else {
      return true;
    }
  }
  return false;
}


// If there is no interference, robot will drive forward and turn 90 degrees. 
// If interfered, robot will drive forward and then attempt to drive backwards. 
void interfered_example() {
 chassis.set_drive_pid(24, DRIVE_SPEED, true);
 chassis.wait_drive();

 if (chassis.interfered) {
   tug_in_case(20);
   return;
 }

 chassis.set_turn_pid(90, TURN_SPEED);
 chassis.wait_drive();
}
//PID tuning with slew
void pid_drive_tune()
{
  tuning_constants();
  front_fork_toggle(OPEN);
  pros::delay(2000);
  //one_mogo_constants();
  //two_mogo_constants();
 
  chassis.set_swing_pid(ez::LEFT_SWING, 90, SWING_SPEED);
  chassis.wait_drive();
  
  pros::lcd::print(3, "%f", chassis.swingPID.error);
  
  
}

void pid_turn_tune()
{
  tuning_constants();
  
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  
  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
}

void pid_swing_tune()
{
  tuning_constants();

  chassis.set_swing_pid(ez::LEFT_SWING, 90, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_swing_pid(ez::LEFT_SWING, 0, SWING_SPEED);
  chassis.wait_drive();
}


void DeployFront()
{
  just_right_fork.set(true);
  pros::delay(155);
  front_fork_toggle(OPEN);
}
void david_auto()
{
  pros::Task deploy_front_task(DeployFront);
  chassis.set_drive_pid(56, 127);
  chassis.wait_until(43);
  
  chassis.set_swing_pid(ez::RIGHT_SWING, -270, 127);
  chassis.wait_until(-60);
  
  chassis.set_turn_pid(-180, 127);
  chassis.wait_drive();

  
}
void tall_goal_rush()
{
  pros::Task deploy_front_task(DeployFront);
  chassis.set_drive_pid(56, 127);
  chassis.wait_until(43);

  chassis.set_drive_pid(-28, 127);
  chassis.wait_drive();
  
  if(tug_in_case(-28))
  {
    return;
  }
  front_fork_toggle(CLOSE);
  
  front_mogo_constants();
  chassis.set_turn_pid(-63, TURN_SPEED);
  chassis.wait_drive();
  
  tall_goal_mech.set(true);
  pros::delay(300);
  back_fork_toggle(OPEN);

  chassis.set_drive_pid(-30, DRIVE_SPEED, true);
  chassis.wait_drive();

  back_fork_toggle(CLOSE);
  pros::delay(500);
  chassis.set_turn_pid(30, TURN_SPEED);
  chassis.wait_drive();

  set_intake(INTAKE);
  pros::delay(1000);
  chassis.set_drive_pid(40, 50, true);
  chassis.wait_drive();
  

  chassis.set_drive_pid(-50, 127);
  chassis.wait_drive();

}

void right_with_win_point()
{
  pros::Task deploy_front_task(DeployFront);
  chassis.set_drive_pid(45, 127);
  chassis.wait_until(35);

  
  chassis.set_drive_pid(-35, 127);
  chassis.wait_drive();

  if(tug_in_case(-35))
  {
    return;  
  }

  tall_goal_mech.set(true);
  pros::delay(300);
  back_fork_toggle(OPEN);
  front_fork_toggle(CLOSE);
}

void left_with_win_point()
{
  
  pros::Task deploy_front_task(DeployFront);
  chassis.set_drive_pid(50, 127);
  chassis.wait_until(38);

  
  chassis.set_drive_pid(-24, 127);
  chassis.wait_drive();

  if(tug_in_case(-24))
  {
    return;  
  }

  tall_goal_mech.set(true);
  pros::delay(300);
  back_fork_toggle(OPEN);
  front_fork_toggle(CLOSE); 

  front_mogo_constants();

  chassis.set_turn_pid(-25, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-16, DRIVE_SPEED, true);
  chassis.wait_drive();

  back_fork_toggle(CLOSE);
  pros::delay(500);
  chassis.set_drive_pid(26, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(78, TURN_SPEED);
  chassis.wait_drive();

  set_intake(INTAKE);
  pros::delay(500);

  chassis.set_drive_pid(45, 50, true);
  chassis.wait_drive();


}

void win_point()
{
  pros::Task deploy_front_task(DeployFront);
}