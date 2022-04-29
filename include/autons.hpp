#pragma once

#include "EZ-Template/drive/drive.hpp"

extern Drive chassis;

//autos
void tall_goal_rush();
void right_with_win_point();
void left_with_win_point();
void david_auto();
void win_point();
void pid_drive_tune();
void pid_turn_tune();
void pid_swing_tune();
void interfered_example();


//tuning values
void default_constants();
void one_mogo_constants();
void two_mogo_constants();
void exit_condition_defaults();
void modified_exit_condition();