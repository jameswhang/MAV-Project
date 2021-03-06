#include "logger.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


int main(int argv, char **argc)
{
  ros::init (argv, argc, "logger");
  Logger logger;

  cout << "Input vision log filename: "<<endl;
  cin >> logger.vision_logname;
  cout << "Input LPE log filename: " <<endl;
  cin >> logger.lpe_logname;
  cout << "Input Velocity log filename: " <<endl;
  cin >> logger.v_logname;

  logger.clearLog();
  ros::Rate loop_rate(100);

  while(ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }

if (logger.px_lpe.size()>0)
{
  plt::figure(1);
  plt::plot(logger.px_lpe, logger.py_lpe);
  plt::figure(2);
  plt::plot(logger.pz_lpe);
//  plt::plot(logger.px_lpe, logger.time_lpe);
//  plt::figure();
//  plt::plot(logger.py_lpe, logger.time_lpe);

}

if(logger.px_vision.size()>0)
{
  plt::figure(3);
  plt::plot(logger.px_vision,logger.py_vision);
}
//  plt::show();
 plt::show();
  return 0;
}


Logger::Logger()
{
  //mag_sub = nh.subscribe("/mavros/imu/mag",1000,&Logger::magCallback, this);
  lpe_sub = nh.subscribe("/mavros/local_position/pose",1000, &Logger::lpeCallback, this);
  vision_sub = nh.subscribe("/mavros/vision_pose/pose",1000, &Logger::visionCallback,this);
  lpev_sub =  nh.subscribe("mavros/local_position/velocity",1000, &Logger::lpeCallback, this);
  // bat_sub  =  nh.subscribe ("mavros/battery",100,&Logger::batCallback,this);
}

Logger::~Logger()
{
  ros::shutdown();
}

void Logger::clearLog()
{
  char* logname_w = new char[lpe_logname.size()+1];
  std::copy(lpe_logname.begin(),lpe_logname.end(), logname_w);
  logname_w[lpe_logname.size()]='\0';
  char* full_name= strcat(logname_w,".csv");
  if (remove(full_name)!=0)
    perror("Error Deleting File");
  else
    puts("Delte lpe log");

  logname_w = new char[vision_logname.size()+1];
  std::copy(vision_logname.begin(),vision_logname.end(), logname_w);
  logname_w[vision_logname.size()]='\0';
  full_name= strcat(logname_w,".csv");
  if (remove(full_name)!=0)
    perror("Error Deleting File");
  else
    puts("Delete vision log");

// // remove buffer
//     if (remove("mag.csv")!=0)
//       perror("Error Deleting File");
//     else
//       puts("Delte magnetic_field log");


}

void Logger::lpeCallback(const geometry_msgs::PoseStamped lpe)
{


  lpe_msgs.px = lpe.pose.position.x;
  lpe_msgs.py = lpe.pose.position.y;
  lpe_msgs.pz = lpe.pose.position.z;

  px_lpe.push_back(lpe.pose.position.x);
  py_lpe.push_back(lpe.pose.position.y);
  pz_lpe.push_back(lpe.pose.position.z);
  //time_lpe.push_back(lpe.header.stamp.sec);

  ofstream myfile;
  char* logname_w = new char[lpe_logname.size()+1];
  std::copy(lpe_logname.begin(),lpe_logname.end(), logname_w);
  logname_w[lpe_logname.size()]='\0';
  char* full_name= strcat(logname_w,".csv");

  myfile.open(full_name,ios::in|ios::app);
  myfile<< fixed << setprecision(4)<<lpe_msgs.px<<',';
  myfile<< fixed << setprecision(4)<<lpe_msgs.py<<',';
  myfile<< fixed << setprecision(4)<<lpe_msgs.pz<<',';
  myfile<< fixed << setprecision(6)<<lpe.pose.orientation.w<<',';
  myfile<< fixed << setprecision(6)<<lpe.pose.orientation.x<<',';
  myfile<< fixed << setprecision(6)<<lpe.pose.orientation.y<<',';
  myfile<< fixed << setprecision(6)<<lpe.pose.orientation.z<<',' <<endl;

  myfile.close();
}


void Logger::lpevCallback(const geometry_msgs::TwistStamped v)
{

  ofstream myfile;
  char* logname_w = new char[v_logname.size()+1];
  std::copy(v_logname.begin(),v_logname.end(), logname_w);
  logname_w[v_logname.size()]='\0';
  char* full_name= strcat(logname_w,".csv");

  myfile.open(full_name,ios::in|ios::app);
  myfile<< fixed << setprecision(4)<< v.twist.linear.x<<',';
  myfile<< fixed << setprecision(4)<< v.twist.linear.y<<',';
  myfile<< fixed << setprecision(4)<< v.twist.linear.z<<',';
  myfile.close();
}

// void Logger::flowCallback(const px_comm::OpticalFlow flow)
// {
//   // ROS_INFO("Logging FLOW");
//   flow_msgs.vx = flow.velocity_x;
//   flow_msgs.vy = flow.velocity_y;
//   flow_msgs.pz = flow.ground_distance;
//   flow_msgs.quality = flow.quality;
//   // logger.csv_dump(flow_logname.c_str());
//   ofstream myfile;
//   char* logname_w = new char[flow_logname.size()+1];
//   std::copy(flow_logname.begin(),flow_logname.end(), logname_w);
//   logname_w[flow_logname.size()]='\0';
//   char* full_name= strcat(logname_w,".csv");
//   myfile.open(full_name,ios::in|ios::app);
//   myfile<< fixed << setprecision(4)<<flow_msgs.vx<<',';
//   myfile<< fixed << setprecision(4)<<flow_msgs.vy<<',';
//   myfile<< fixed << setprecision(4)<<flow_msgs.pz<<',';
//   myfile<< fixed << setprecision(4)<<flow_msgs.quality<<','<<endl;
//   myfile.close();
// }

 void Logger::visionCallback(const geometry_msgs::PoseStamped vision){
  vision_msgs.px = vision.pose.position.x;
  vision_msgs.py = vision.pose.position.y;
  vision_msgs.pz = vision.pose.position.z;

  px_vision.push_back(vision.pose.position.x);
  py_vision.push_back(vision.pose.position.y);
  pz_vision.push_back(vision.pose.position.z);


  ofstream myfile;
  char* logname_w = new char[vision_logname.size()+1];
  std::copy(vision_logname.begin(),vision_logname.end(), logname_w);
  logname_w[vision_logname.size()]='\0';
  char* full_name= strcat(logname_w,".csv");
  myfile.open(full_name,ios::in|ios::app);

  myfile<< fixed << setprecision(4)<<vision_msgs.px<<',';
  myfile<< fixed << setprecision(4)<<vision_msgs.py<<',';
  myfile<< fixed << setprecision(4)<<vision_msgs.pz<<',';
  myfile<< fixed << setprecision(6)<<vision.pose.orientation.w<<',';
  myfile<< fixed << setprecision(6)<<vision.pose.orientation.x<<',';
  myfile<< fixed << setprecision(6)<<vision.pose.orientation.y<<',';
  myfile<< fixed << setprecision(6)<<vision.pose.orientation.z<<',' <<endl;

  myfile.close();
}
