#include "ros/ros.h"
#include "px4_offboard/MavState.h"
#include "geometry_msgs/Point.h"


#include "px4_offboard/include.h"
#include "px4_offboard/CtrlPx4.h"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include <string>

enum {takingoff = 0, calibration, tracking, traverse, turning, landing};

class Mission
{
public:

Mission();
~Mission(){};

void publish() { //ROS_INFO("Publishing");
 _mission_ctrl_pub.publish(_objCommand);}; // for timing control

bool getTakeoffFlag(){ return _is_takeoff;     };
void takeoff();
bool getLandFlag(){ return _is_land;     };
void land();
bool turnLeft90();
bool getCalibrateFlag(){return _is_calibrate;};
bool getFailFlag() {return _is_fail;};
void setFlightMode(int request_mode)
{
	if (_flight_mode!=request_mode)
	{
		_flight_mode_prev = _flight_mode;
		_flight_mode = request_mode;
	}
};

void setControlMode(bool pos_ctrl)
{
  _objCommand.control = pos_ctrl;
}

int getFlightMode() {return _flight_mode;};
int getPrevFlightMode(){return _flight_mode_prev;};
void logSp();


private:

ros::NodeHandle _nh;
ros::Subscriber _wall_sub;
ros::Subscriber _obj_sub;
ros::Subscriber _state_sub;
ros::Subscriber	_obst_sub;
ros::Subscriber _lpe_sub;
ros::Subscriber _vel_sub;
px4_offboard::MavState _objCommand;


int  _flight_mode;
int  _flight_mode_prev;

int  _wall_cnt;
int  _obj_cnt;
int  _cannot_find_wall_cnt;
int	 _obst_cnt ; // how many obstacles have the drone run into
bool _obst_found;
int  _safe_dist; // distance to keep away from obstacles / walls


bool _is_takeoff;
bool _is_land;
bool _is_calibrate;
bool _is_fail;



std::ofstream logMissionSp;
static constexpr int _room_size = 2500; //(mm)
static constexpr int _traverse_inc = 10;
float _traverse_height = 0.85;
float _traverse_speed  = 0.1;

// position info
Eigen::Matrix4f _lpe;
Eigen::Vector3f _vel;
float _roll, _pitch, _yaw, _yaw_prev, _angle_rad;

void stateCallback(const px4_offboard::MavState state)
{
	_is_takeoff = state.takeoff;
	_is_land 	= state.land;
	_is_fail 	= state.failsafe;
};

void velCallback(const geometry_msgs::TwistStamped vel_read);
void lpeCallback(const geometry_msgs::PoseStamped pos_read);
void wallCallback(const geometry_msgs::Point ang);
void obstCallback(const geometry_msgs::Point msg);
void objCallback(const geometry_msgs::Point);


void rot2rpy(Matrix3f R,float& r, float& p, float& y);
void resetCommand(px4_offboard::MavState& command);

ros::Publisher _mission_state_pub;
ros::Publisher _mission_ctrl_pub;


px4_offboard::MavState _emptyCommand;

};
