#ifndef ROSHANDLER_H_
#define ROSHANDLER_H_

#include "ros/ros.h"
#include <mavros_msgs/BatteryStatus.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>

#include "frontend/MavState.h"


#include <std_msgs/Float64.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;

class RosHandler{ // this handler needs clean up to make it more generic
public:
	RosHandler();
	~RosHandler();

	// rgbd slam related
	void 	 updateCamPos (double, Matrix4f); // to pixhawk
	void 	 updateLpeLastPose(int id);
	// object detection and navigation related
	void 	 updateObjPos  (geometry_msgs::Point);
	void 	 updateWallPos (geometry_msgs::Point);
	void 	 updateObstacleDistance(geometry_msgs::Point obsDist);

	// mavros related
	Matrix4f getLpe();
	Matrix4f fuseLpeTm(Matrix4f,int from, int to);  // need to be consistent with pixhawk
	// void 	 getTm(Matrix4f& tm, Matrix<float, 6, 6>& im, double& dt);

	Matrix4f 	 getTmFromIdtoId(int from, int to);
	bool 	 getTakeoffFlag(){ return _is_takeoff;};
	bool 	 getArmFlag(){ return _is_arm;};

	// double   getTime() 				{ return _time; };
	// bool 	  getLpeValid()				{ return _lpe_valid; };


private:
	ros::NodeHandle 			_nh;
	ros::Publisher 				_rgbd_slam_pub; // go to mavros
	ros::Publisher				_object_pub;    // go to px4_offboard
	ros::Publisher				_wall_pub;	// go to px4_offboard
	ros::Publisher				_obst_pub;	// go to px4_offboard

	ros::Subscriber 			_state_sub;
	ros::Subscriber				_px4_offboard_sub;
	ros::Subscriber				_lpe_sub;
	ros::Subscriber				_flow_valid_sub;
	ros::Subscriber				_bat_sub;

	bool 						_lpe_valid;    // valid flag

	bool 						_is_takeoff;
	bool 						_is_land;
	bool						_is_fail;
	bool 						_is_arm; // note down arming instance is important for some handling
	double						_timeout;
	double 						_time;     	// time stamp


	std::vector<Matrix4f> _lpe_nodes;
	// note down the stream of lpe
	// format as time, roll, pitch, yaw

	Matrix4f 						_lpe;									// current lpe
	// Matrix4f					_lpe_prev_cam; 		// last camera node lpe
	// double						_time_prev_cam;

	// Matrix4f					_lpe2cam;   			// transformation matrix

	Vector3f					_xyz;

	geometry_msgs::PoseStamped 	_rgbd_slam_pos;

	void lpeCallback(const geometry_msgs::PoseStamped pos_read);
	void flowValidCallback(const std_msgs::Float64 data);
	void batCallback(const mavros_msgs::BatteryStatus bat);


// rotation utilities
	void q2rpy		(Quaternionf q, float& r, float& p, float& y);
	void rot2rpy	(Matrix3f R,float& r, float& p, float& y);
	Matrix3f rpy2rot(float r, float p, float y);

	void stateCallback(const frontend::MavState state)
	{
	_is_takeoff = state.takeoff;
	_is_land 	= state.land;
	_is_fail 	= state.failsafe;
	_is_arm   = state.arm;
	};


};


#endif
