#include "intake_pto.hpp"
#include "autons.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

pros::Task anti_jam_task(anti_jam, nullptr);

bool isIntakeOn = false;
bool isJammed = false;
bool last_pto_state = false;
bool switched = false;
bool get_intake_toggle() { return pto_intake_pistion.get(); }


void toggle_intake(bool toggle) {
  last_pto_state = toggle;
  chassis.pto_toggle({chassis.left_motors[2], chassis.right_motors[2]}, toggle);
  pto_intake_pistion.set(toggle);
  
  if (toggle) {
    switched = true;
    chassis.left_motors[2].set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    chassis.right_motors[2].set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  }
  else {
    switched = false;
  }
  
}

void set_intake(int power)
{
    if(!get_intake_toggle()) return;
    chassis.left_motors[2].move(power);
    chassis.right_motors[2].move(power);
    if(power < 0) 
    {
        anti_jam_task.resume();
    }
    else anti_jam_task.suspend();
}
void anti_jam(void*)
{
  isJammed = false;
  int jamCounter = 0;
  int waitTime = 250;
  
  while(true)
  {
    if(!get_intake_toggle()) continue;
    if(switched)
    {
        pros::delay(250);
        switched = false;
    }
    if(isJammed)
    {
      set_intake(60);
      jamCounter+=ez::util::DELAY_TIME;
      if(jamCounter > waitTime)
      {
        isJammed = false; 
        jamCounter = 0;
        set_intake(-127);
      }
    }
    else if(isIntakeOn && chassis.left_motors[2].get_actual_velocity() == 0 && chassis.right_motors[2].get_actual_velocity() == 0)
    {
    
      jamCounter+= ez::util::DELAY_TIME;
      if(jamCounter > waitTime)
      {
        jamCounter = 0;
        isJammed = true;
      }
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}

void intake_control()
{
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
    {
        toggle_intake(!get_intake_toggle());
    }

    if(get_intake_toggle())
    {
        set_intake(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) ? -127 : 127);
    }
}