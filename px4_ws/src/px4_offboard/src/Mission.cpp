#include <unistd.h>
#include "px4_offboard/Mission.h"

// flight logic / state machine handler

enum {takingoff, calibration,  tracking, traverse, turning, landing};

int  crash_cnt = 0;
bool propelled = 0;

Mission::Mission()
{
	_flight_mode  = takingoff; // start with takeoff
	_flight_mode_prev = takingoff;

	_is_takeoff   = 0;
	_is_land = 0;
	_is_calibrate = 0;
	_is_fail = 0;

	_safe_dist = 1800; // mm (change it back!)
	_obst_found = 0;
	_wall_cnt = _obj_cnt = 0;
	_cannot_find_wall_cnt = 0;
	_roll = _pitch = _yaw = 0;

	_lpe.setIdentity();
	_vel.setZero();
	resetCommand(_emptyCommand); // hover command

	_mission_state_pub = _nh.advertise<px4_offboard::CtrlState> ("/obj/main_state",100);
	_mission_ctrl_pub = _nh.advertise<px4_offboard::JoyCommand> ("/obj/cmd_mav",10);

	_state_sub = _nh.subscribe("/px4_offboard/state",  10,  &Mission::stateCallback, this);
	_lpe_sub   = _nh.subscribe("/mavros/local_position/pose", 100, &Mission::lpeCallback,this);
	_vel_sub   = _nh.subscribe("/mavros/local_position/velocity", 100, &Mission::velCallback,this);

	_wall_sub  = _nh.subscribe("/objDetect/wall_pose", 100, &Mission::wallCallback, this);
	_obj_sub   = _nh.subscribe("/objDetect/obj_pose",   100, &Mission::objCallback, this);
	_obst_sub  = _nh.subscribe("/objDetect/obst_dist",100,&Mission::obstCallback,this);

}



int main(int argc, char** argv)
{
  ros::init(argc, argv, "mission_planner");
  int takeoff_set = 0; //takeoff flag only set once
  int printDelay = 0;
  int printMod = 400;
  Mission mission;
  usleep(1000*1000);

  ros::Rate loop_rate(100);

  while(ros::ok())
  {

  		printDelay++;
  		if (mission.getFailFlag())
  		{
  			ROS_INFO("[Mission] Failure detected");
  			return 0; // override mission command
  		}

		switch(mission.getFlightMode())
	  	{
	  		case takingoff:
	  			if (mission.getTakeoffFlag())
	  			{
	  				ROS_INFO("[Mission] Finished taking off");
					mission.setFlightMode(calibration);
		  		}
		  		else
		  		{
		  			if (!(printDelay%printMod))
	  					ROS_INFO("[Mission] Takeoff mode");

					if (takeoff_set < 10) // limited amount of time
		  			{
						ROS_INFO("[Takeoff] flag set %d",takeoff_set);
						takeoff_set++;
						mission.takeoff();
					}
		  		}

	  		break;

	  		case calibration:
	  			if (!(printDelay%printMod))
	  				ROS_INFO("[Mission] Calibration mode");

	  			if(!mission.getCalibrateFlag()) // then wait for state transition
		  		{
					usleep(10);
				}

	  		break;

	  		case traverse:
	  		    if (!(printDelay%printMod))
	  				ROS_INFO("[Mission] Traversing mode");
	  		break;

	  		case tracking:
	  		    if (!(printDelay%printMod))
	  				ROS_INFO("[Mission] Tracking mode");

	  		break;

	  		case turning:
	  			if (!(printDelay%printMod))
	  				ROS_INFO("[Mission] Turning mode");

	  			if(mission.turnLeft90()) {
		  			mission.setFlightMode(mission.getPrevFlightMode()); // go back to previous flight mode
					ROS_INFO("[Mission]Finished turning !!");
				}
		  		else
		  			mission.publish();

		  	break;

	  		case landing:

	  		    if(mission.getLandFlag())
	  		    {
	  		    	ROS_INFO("[Mission] Landed");
	  		    	return 0;
	  		    }
	  		    else if(!(printDelay%printMod))
	  		    {
		  		ROS_INFO("[Mission] Landing mode");
	  		    }
	  		    mission.land();

	  		break;
	  	}

		ros::spinOnce();
	  	loop_rate.sleep();

  }
}


void Mission::takeoff()
{
	resetCommand(_objCommand);
	_objCommand.takeoff = 1;
	_objCommand.arm = 1;
 	_objCommand.offboard = 1;
	_mission_ctrl_pub.publish(_objCommand);
};

void Mission::land()
{
	resetCommand(_objCommand);
	_objCommand.takeoff = 0;
	_objCommand.land =1 ;
	_mission_ctrl_pub.publish(_objCommand);
}

bool Mission::turnLeft90()
{
	if ((_yaw_prev > 0.5 * M_PI)&&(_yaw < 0))
	{
		_yaw += 2* M_PI; // warp around
	}

	resetCommand(_objCommand);
	if  (_lpe(2,3)>1.2)
	{
		ROS_INFO("[Mission] correct height");
		_objCommand.position.z = 0.1*(-_lpe(2,3) + 1); // make sure yaw don't jump
	}

	_objCommand.yaw = 0.5 * fabs(_yaw - _yaw_prev - 0.5*M_PI);

	return (fabs(_yaw - _yaw_prev - 0.5 * M_PI) < 0.25); // true if finished turning (set this threshold to be higher if overturn)
}

void Mission::objCallback(const geometry_msgs::Point pos)
{
	_obj_cnt ++ ;

	const float Kp = 0.25; // tracking gain
	if ((_obj_cnt > 4)&&(_flight_mode != landing) && (_flight_mode!=takingoff))
	{
		setFlightMode(tracking);
		// _flight_mode = tracking;

		resetCommand(_objCommand);
		_objCommand.position.y = Kp * (pos.z - 1200.0) / 1200.0; // keep 1m distance away from target
		_objCommand.position.x = Kp * (pos.x - 160.0)  / 200.0;
		_objCommand.position.z = Kp * (pos.y - 120.0)  / 200.0;
		_objCommand.yaw = - Kp * angle_rad;


		// ROS_INFO("Forward %3.2f Left %3.2f Up %3.2f", _objCommand.position.y, _objCommand.position.x, _objCommand.position.z);

		publish();
		if ((pos.z < 1200) && (abs(pos.x-160) < 35 )&& (abs(pos.y-120)< 35)) //center arranged
		{
			ROS_INFO("[Mission] Found object ><><><><>< Land!");
			resetCommand(_objCommand);
			_objCommand.land = 1;
			setFlightMode(landing);

		}
	}

}


void Mission::wallCallback(const geometry_msgs::Point ang)
{
	const float K = 0.3; //listen to wall if in calibration mode

	if((_flight_mode == calibration) && (!_is_calibrate))
	{
		angle_rad = ang.x;
		if(angle_rad == -5000 || angle_rad > 0.785 || angle_rad < -0.785)
		{
				ROS_INFO("[Mission] Cannot find wall");
				_cannot_find_wall_cnt++;
				if(_cannot_find_wall_cnt >100) // if cannot find wall for too long land
				{
					resetCommand(_objCommand);
					setFlightMode(landing);
					_objCommand.land = 1;


				}

		}
		else
		{
			_cannot_find_wall_cnt = 0;
			if( (fabs(angle_rad) > 0.11) && (!_is_calibrate) )					// P controller to adjust drone to a found wall
			{
				ROS_INFO("[Mission] Adjust Angle %3.2f", angle_rad);
				resetCommand(_objCommand);
				_objCommand.yaw = - K * angle_rad;
				 publish();
				_wall_cnt = 0;
			}
			else
			{

				_wall_cnt++;
				if(_wall_cnt > 5){
				resetCommand(_objCommand);
				_objCommand.yaw = - K * angle_rad;
				_objCommand.position.y = 0.05 * (ang.z-_safe_dist)/1000.0; // move to 1.5 from obstacles
				 ROS_INFO("[Mission] Adjust Distance %3.2f",_objCommand.position.y);
				 publish();
				}

				if((_wall_cnt > 10) && (fabs(ang.z - (float)_safe_dist) < 20))
				{
					ROS_INFO("[Mission] Wall is perpendicular now.");   // drone is perpendicular, hover now
					if (!_is_calibrate)
					{
						_is_calibrate = 1;
						_yaw_prev = _yaw;
						ROS_WARN("[Mission] Yaw pin down is %f, z pin down is %f", _yaw_prev,_lpe(2,3));
						resetCommand(_objCommand);
						if (_lpe(2,3) > 0.7){
							_objCommand.position.z = -_lpe(2,3) + 0.7; // go down to around 1 m
							publish();
							usleep(1000*2000);
						}
					}  // issue this setpoint once

				}

			}
		}
	}
	else if ((_flight_mode == calibration) && (_lpe(2,3) < 1.1)) // after going down: turn 90 degrees
	{
		ROS_INFO("[Mission] Start turning after going down. What's the height right now? %3.2f", _lpe(2,3));
		setFlightMode(traverse); // so that after turning it will return to traverse rather than calibration
		setFlightMode(turning);
		_safe_dist = 1300;

	}

};

void Mission::obstCallback(geometry_msgs::Point msg)
{
	if ((_flight_mode > tracking) && (msg.y < 900))//failsafe object detect
	{
		crash_cnt++;
		if(crash_cnt>5){
			setFlightMode(landing);
			ROS_ERROR("[Mission] obstacles too close detected. Land");
		}
	//_objCommand.land = 1;

	}
	else
		crash_cnt = 0;
	if (_flight_mode == traverse) // maybe want some threshold in case wrong depth occur
	{
		if ((msg.x > 10)) // check bad minimum distance value
		{
			resetCommand(_objCommand);
			if (msg.y < _safe_dist)
			{
				if (!_obst_found)
				{
					propelled = 0;
					_obst_found = 1;
					_obst_cnt++;
					ROS_INFO("[Mission] Find an obstacle: Turning 90 degrees"); //
					if (_safe_dist < 2500){
						_safe_dist += 50;
					}
					if (_lpe(2,3) > 0.7){
						_objCommand.position.z = -_lpe(2,3) + 0.7; // go down to around 1 m
						publish();
						usleep(1000*2000);
					}
					_yaw_prev = _yaw;
					setFlightMode(turning); // enter turning mode

				}
			}
			else
			{

				resetCommand(_objCommand);

				if (!(_obst_cnt%2)) // prior knowledge
				{
					ROS_INFO ("[Mission] calibrate yaw from obstacle");
					_objCommand.yaw = - 0.3 * angle_rad; //tuning here needed
				}

				_objCommand.position.y = 0.1 * (msg.z - _safe_dist)/1000.0; // gradually move to the obstacle
				ROS_WARN("[Mission] ===== careful proceed ---- <<<< %3.2f", _objCommand.position.y);
				publish();
			}


		}
		else
		{
			_obst_found = 0;
			if ((_vel(1)*_vel(1)+_vel(0)*_vel(0)) < 0.04)
			{

				resetCommand(_objCommand);
				if(propelled)
				{

					_objCommand.position.y = 0.1; // go forward
					std::cout<< "small vel" << std::endl;

					publish();
				}
				else
				{
					// publish();
                                	// usleep(2000*1000); // don't over react here

					propelled = 1;
					_objCommand.position.y = 0.6;

					 publish();
	                                usleep(1000*1000); // don't over react her
					std::cout<< "big vel" << std::endl;
				}
				//publish();
				//usleep(2000*1000); // don't over react here
			}
		}
	}

}

void Mission::velCallback(const geometry_msgs::TwistStamped vel_read)
{
	_vel.setZero();
	_vel(0) = vel_read.twist.linear.x;
	_vel(1) = vel_read.twist.linear.y;
	_vel(2) = vel_read.twist.linear.z;
	// ROS_INFO("squared xy velocity is %3.2f", _vel(1)*_vel(1)+_vel(0)*_vel(0));

}

void Mission::lpeCallback(const geometry_msgs::PoseStamped pos_read)
{

	Eigen::Quaternionf q (pos_read.pose.orientation.w, pos_read.pose.orientation.x, pos_read.pose.orientation.y,pos_read.pose.orientation.z);
	_lpe.setIdentity(); 	// clear buffer
	_lpe.topLeftCorner (3,3) = q.matrix();
	_lpe.topRightCorner(3,1) << pos_read.pose.position.x, pos_read.pose.position.y, pos_read.pose.position.z;
	rot2rpy(q.matrix(), _roll, _pitch, _yaw);

}
void Mission::resetCommand(px4_offboard::JoyCommand& command)
{
	command.takeoff = 0;
	command.land = 0;
	command.arm = 1;
	command.offboard = 1;
	command.position.x = 0;
	command.position.y = 0;
	command.position.z = 0;
	command.yaw = 0;
}

inline void Mission::rot2rpy(Matrix3f R,float& r, float& p, float& y)
{
	float beta, alpha, gamma;
	beta = atan2(-R(2,0), sqrt(R(0,0)*R(0,0)+R(1,0)*R(1,0)));

	if(fabs(beta - M_PI/2) < 0.001){
	alpha = 0;
	gamma = atan2(R(0,1),R(1,1));
	}
	else if (fabs(beta + M_PI/2) < 0.001){
	alpha = 0;
	gamma = -atan2(R(0,1),R(1,1));
	}
	else{
	alpha = atan2(R(1,0), R(0,0));
	gamma = atan2(R(2,1), R(2,2));
	}

	y = alpha ;
	p = beta  ;
	r = gamma ;
}

// inline bool Mission::turnLeft90() // maybe do something?
// {

// }

// takes off really high
// see the wall
// get perpendicular to the wall
// go towards it
//
// go to apropriate height1
//
// <change_height>
//
// <walk arround>
// turn 90
// go towards 2nd wall until distance d(i)
//
// turn 90
// go towards 3nd wall until distance d(i)
//
// turn 90
// go towards 4nd wall until distance d(i)
//
// turn 90
// go towards 1st all until distance d(i)
//
// go to <walk arround>
//
// if walked all possible distances go to change_height
//
