#pragma once
#include "EZ-Template/piston.hpp"
#include "main.h"

inline Piston back_fork('G');
inline Piston front_fork('F');

inline void Back_Fork_Toggle(bool toggle) { back_fork.set(toggle); }
inline void Front_Fork_Toggle(bool toggle) { front_fork.set(toggle); }