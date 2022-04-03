#pragma once
#include "EZ-Template/piston_group.hpp"
#include "main.h"

inline Piston pto_intake_pistion('H');

void toggle_intake(bool toggle);
void set_intake(int power);
void anti_jam(void*);
void intake_control();
inline bool get_intake_toggle();
extern bool isIntakeOn;
extern bool isJammed;

extern pros::Task anti_jam_task;
