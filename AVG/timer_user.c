
#include "timer_user.h"
#include <math.h>
extern RC_ctrl_t rc_ctrl;

fp64 Angle_last[4];
fp64 Angle_erra[4];
fp64 Angle_target[4];

fp64 Angle_erra_correct[4];

fp64 Angle_Helm_Target[4];
fp64 Speed_Motor_Target[4];

fp64 M2006_Target_1, M2006_Target_2, M2006_Target_3, M2006_Target_4;
fp64 M3508_Target_1, M3508_Target_2, M3508_Target_3, M3508_Target_4;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		if (rc_ctrl.rc.s[0] == 3 && rc_ctrl.rc.s[1] == 3) //底盘正常模式
		{
				int16_t vtx=8*rc_ctrl.rc.ch[2];
				int16_t vty=8*rc_ctrl.rc.ch[3];
			  int16_t vtw=8*rc_ctrl.rc.ch[0];
			
			Speed_Motor_Target[0] = sqrt( pow( vty-0.7071*vtw,2)+pow(vtx-0.7071*vtw,2));
			Speed_Motor_Target[1] = sqrt( pow( vty-0.7071*vtw,2)+pow(vtx+0.7071*vtw,2));
			Speed_Motor_Target[2]= sqrt( pow( vty+0.7071*vtw,2)+pow(vtx+0.7071*vtw,2));
			Speed_Motor_Target[3] = sqrt( pow( vty-0.7071*vtw,2)+pow(vtx+0.7071*vtw,2));
			
		if(!(vtx==0&&vty==0&&vtw==0))
		{
		Angle_target[0]=atan2(vty-0.7071*vtw ,vtx-0.7071*vtw);
    Angle_target[1]=atan2(vty+0.7071*vtw ,vtx+0.7071*vtw);
		Angle_target[2]=atan2(vty-0.7071*vtw ,vtx+0.7071*vtw);
		Angle_target[3]=atan2(vty+0.7071*vtw ,vtx-0.7071*vtw);
		
		for(int a=0;a<4; a++)
		{
			
		 Angle_erra[a]=1.570796-Angle_target[a];
			
			if( (3.141593>=Angle_erra[a]) && (Angle_erra[a]>=0) )
			{
			
			Angle_erra_correct[a]=Angle_erra[a];
			
			}
			else if( (-3.141593<=Angle_erra[a]) && (Angle_erra[a]<0) )
			{
			
			Angle_erra_correct[a]=-Angle_erra[a];

			}
			else if( (3.141593<Angle_erra[a]) && ( Angle_erra[a]<=6.283185 ) )
			{
				
			Angle_erra_correct[a]=-6.283185+Angle_erra[a];
			
			}
			else if( (-3.141593>Angle_erra[a]) && ( Angle_erra[a]>=-2*3.141593 ) )
			{
			
			Angle_erra_correct[a]=6.283185+Angle_erra[a];
				
			}
			
			Angle_Helm_Target[a]=Angle_erra_correct[a]*4367.18784;
					
		}

		}			
	
		else if(vtx==0&&vty==0&&vtw==0)
		{
		
		
		for(int a=0;a<4; a++)
		{
		
		Angle_Helm_Target[a]=0;
			
		}
		
		
		
		}
			
			

			M2006_Target_1 = pid_call_2(Angle_Helm_Target[0],1);
			M2006_Target_2 = pid_call_2(Angle_Helm_Target[1],2);
			M2006_Target_3 = pid_call_2(Angle_Helm_Target[2],3);
			M2006_Target_4 = pid_call_2(Angle_Helm_Target[3],4);

			M3508_Target_1 = PID_velocity_realize_1(Speed_Motor_Target[0],1);
			M3508_Target_2 = PID_velocity_realize_1(Speed_Motor_Target[1],2);
			M3508_Target_3 = PID_velocity_realize_1(Speed_Motor_Target[2],3);
			M3508_Target_4 = PID_velocity_realize_1(Speed_Motor_Target[3],4);

	    CAN2_CMD_1(M2006_Target_1, M2006_Target_2, M2006_Target_3, M2006_Target_4);
			CAN1_CMD_1(M3508_Target_1, M3508_Target_2, M3508_Target_3, M3508_Target_4);


		
		}
	}
}
