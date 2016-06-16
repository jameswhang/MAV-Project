#include "ros/ros.h"
#include <sstream>
#include "geometry_msgs/PoseStamped.h"
#include <string.h>

#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <mavros_msgs/CommandBool.h>

double roll, pitch, yaw;
ros::NodeHandle n;
mavros_msgs::CommandBool ARM;
void RPYCallBack(const geometry_msgs::PoseStamped data);

ros::Subscriber subscriber = n.subscribe("/mavros/local_position/pose",100,RPYCallBack);
ros::ServiceClient client = n.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");



void RPYCallBack(const geometry_msgs::PoseStamped data){

ROS_INFO("Quaternion: x: %f, y: %f, z: %f, w: %f", data.pose.orientation.x, data.pose.orientation.y,data.pose.orientation.z,data.pose.orientation.w);

tf::Quaternion q(data.pose.orientation.x,data.pose.orientation.y,data.pose.orientation.z,data.pose.orientation.w);
tf::Matrix3x3 m(q);

m.getRPY(roll,pitch,yaw);

ROS_INFO("Euler: roll: %f, pitch: %f, yaw: %f", roll, pitch, yaw);

if(roll > 1.57/2 || pitch > 1.57/2 || yaw > 1.57/2){ // disarm
ROS_INFO("Disarming");
ARM.request.value = false; 
client.call(ARM);
}

}





int main(int argc, char **argv){
ros::init(argc, argv, "RPY_g");
int n = 10;

while(n--){
ARM.request.value = true;
client.call(ARM);
}
ros::spin();

return 0;


}






