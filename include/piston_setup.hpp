#pragma once
#include "EZ-Template/piston.hpp"
#include "EZ-Template/piston_group.hpp"
#include "main.h"
#include "pros/adi.hpp"

inline Piston back_fork('G');
inline Piston just_right_fork('F');
inline Piston tall_goal_mech('D');
inline Piston preload_release('C');
inline PistonGroup front_fork({'F', 'E'});
inline void back_fork_toggle(bool toggle) { back_fork.set(!toggle); }
inline void front_fork_toggle(bool toggle) { front_fork.set(toggle); }