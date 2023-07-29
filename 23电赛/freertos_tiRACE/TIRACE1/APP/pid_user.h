#ifndef __PID_USER_H
#define __PID_USER_H
#include "pid.h"
#include "includes.h"


void PID_devices_Init(void);

fp32 PID_velocity_realize_1(fp32 set_speed,int i);
 
 fp32 PID_postions_realize_1(fp32 set_encode,int i);

#endif























