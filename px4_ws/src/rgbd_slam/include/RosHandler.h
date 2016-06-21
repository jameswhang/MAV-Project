#ifndef ROSHANDLER_H_
#define ROSHANDLER_H_

#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "Eigen/Core"
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;

class RosHandler{
public:
	RosHandler();
	~RosHandler();
	void updatePos(Matrix4f&);
	Matrix4f getLpe(){ return _lpe; };

private: 
	ros::NodeHandle 			_nh;
	ros::Publisher 				_rgbd_slam_pub;
	ros::Subscriber				_lpe_sub;

	Matrix4f 				_lpe;

	geometry_msgs::PoseStamped 	_rgbd_slam_pos; 
	void lpeCallback(const geometr1y_msgs::PoseStamped pos_read);
	void q2rpy(Quaternionf q, float& r, float& p, float& y);
	void rot2rpy(Matrix3f R,float& r, float& p, float& y);

};


#endif
