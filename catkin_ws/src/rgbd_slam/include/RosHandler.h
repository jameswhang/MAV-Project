#ifndef ROSHANDLER_H_
#define ROSHANDLER_H_

#include "ros/ros.h"
#include <mavros_msgs/BatteryStatus.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float64.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;

class RosHandler{
public:
	RosHandler();
	~RosHandler();
	void updatePos(Matrix4f&);
	Matrix4f getLpe(){ return _lpe; };
	double   getTime(){ return _lpe_time; };

private: 
	ros::NodeHandle 			_nh;
	ros::Publisher 				_rgbd_slam_pub;
	ros::Subscriber				_lpe_sub;
	ros::Subscriber				_gpe_sub;
	ros::Subscriber				_bat_sub;
	
	bool 					_lpe_valid;    // valid flag
	double 					_lpe_time;     // time stamp
	double					_lpe_timeout;  
	Matrix4f 				_lpe;
	// Matrix4f				_lpe2cam;   // transformation matrix

	Vector3f				_rpy;
	Vector3f				_xyz;



	geometry_msgs::PoseStamped 	_rgbd_slam_pos; 
	void lpeCallback(const geometry_msgs::PoseStamped pos_read);
	void gpeCallback(const std_msgs::Float64 data);
	void batCallback(const mavros_msgs::BatteryStatus bat);

	void q2rpy(Quaternionf q, float& r, float& p, float& y);
	void rot2rpy(Matrix3f R,float& r, float& p, float& y);		
	Matrix3f rpy2rot(float r, float p, float y);

};


#endif
