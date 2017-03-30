#include "ros/ros.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include "geometry_msgs/TwistStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include "sensor_msgs/MagneticField.h"
#include "px_comm/OpticalFlow.h"
#include "std_msgs/String.h"

using namespace std;

typedef struct lpe_message
{
  float px;
  float py;
  float pz;

}lpeMsg;

// typedef struct vision_messgae
// {
//   float px;
//   float py;
//   float pz;
//
// }visionMsg;


class Logger
{
public:
  Logger();
  ~Logger();
  void clearLog();
  string vision_logname;
  string lpe_logname;


  // void csv_dump     (const char*);
private:
  // subscriber callback
  void lpeCallback  (const geometry_msgs::PoseStamped);
  void visionCallback (const geometry_msgs::PoseStamped);


  lpeMsg    lpe_msgs;
  // visionMsg   vision_msgs;
  lpeMsg    vision_msgs;
  ros::NodeHandle nh;
  ros::Subscriber lpe_sub;
  ros::Subscriber vision_sub;

};
