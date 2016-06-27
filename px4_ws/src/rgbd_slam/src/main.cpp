/*
 * main.cpp
 *
 *  Created on: May 15, 2015
 *      Author: Gian Danuser & Michael Eugster
 */
#include <iostream>
#include <ctime>
#include "RosHandler.h"

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include "VisualOdometry.h"
#include "AsusProLiveOpenNI2.h"
#include "Backend.h"

using namespace std;
using namespace Eigen;


// #define STREAM_ONLY


// Settings

enum {
        setNewNodeAfterNrOfFailedMatches = 1
};

static constexpr int backendPort = 11000;


#ifdef STREAM_ONLY
int main(int argc, char **argv)
{
	ros::init(argc,argv,"rgbd");
	//
	// initialization
	//
	static Frame frame;
	Matrix4f tmCurToNode = Eigen::Matrix4f::Identity();
	Matrix<float, 6, 6> imCurToNode = Matrix<float, 6, 6>::Identity();
	boost::mutex backendMutex;
	Backend backend(backendPort, backendMutex);
	
	//
	// start the RGBD sensor
//	AsusProLive::start();
	AsusProLiveOpenNI2::start();
	
	//
	// grab some images until brightness is adjusted correctly
	for(int i=0; i < 5; ++i)
	{
//		AsusProLive::grab(frame);
		AsusProLiveOpenNI2::grab(frame);
	}
	
	//
	// wait until backend sets the starting position
	while(!backend.currentPosUpToDate()) boost::this_thread::sleep(boost::posix_time::milliseconds(1));

	//
	// first frame
	double start, end, dif;
	while(backend.running())
	{
		
		//debug
		start = static_cast<double>( clock () ) /  CLOCKS_PER_SEC;
		
//		AsusProLive::grab(frame);
		AsusProLiveOpenNI2::grab(frame);
		
		//
		// debug
		end = static_cast<double>( clock () ) /  CLOCKS_PER_SEC;
		dif = (end - start)*1000.0;
		cout << "Grabbing took " << dif << "ms" << endl;


		//
		//debug
		start = static_cast<double>( clock () ) /  CLOCKS_PER_SEC;
		
		backend.setNewNode(frame,tmCurToNode,imCurToNode,1);
		
		//
		// debug
		end = static_cast<double>( clock () ) /  CLOCKS_PER_SEC;
		dif = (end - start)*1000.0;
		cout << "Sending took " << dif << "ms" << endl;
	}
//	AsusProLive::stop();
	AsusProLiveOpenNI2::stop();
	return 0;	
}

#else

static bool processFrame(const Frame& newFrame, const Frame& prevNode, Matrix4f& tmCurToNode, Matrix<float, 6, 6>& imCurToNode, bool& newNode)
{


	//
	// estimate transforamtion
	bool validFlag;
	VisualOdometry::estimateTrafo(newFrame, prevNode, tmCurToNode, imCurToNode, validFlag);

	// process result
	VisualOdometry::Result res;
	
	if (validFlag){
		res = VisualOdometry::checkReliability(tmCurToNode, newFrame.getTime() - prevNode.getTime());
		validFlag = (res != VisualOdometry::invalid); 
	}

	if (validFlag){
		newNode = (res == VisualOdometry::valid);
		return true;
	}

	else {
		newNode = false;
		return false;
	}
}

enum{newNode = 0, prevNode = 1};

int main(int argc, char **argv)
{	
	ros::init(argc,argv,"rgbd");
	RosHandler logger;


	// initialization
	//
	int state = newNode; 
	static Frame frames[2];
	static Matrix4f tmCurToNode;
	static Matrix<float, 6, 6> imCurToNode;

	// lpe related
	double currentTime = 0;; 
	Matrix4f currentPos, cameraPos;
	Matrix3f currentRM;
	Vector3f currentTV;
	currentPos.setIdentity();
	currentRM.setIdentity();
	currentTV.setZero();

	boost::mutex backendMutex;

	Backend backend(backendPort, backendMutex);
	unsigned int couldNotMatchCounter = 0;
	unsigned int totalNumberOfFrames = 0; // debug
	unsigned int dummyFrameCounter = 0; // debug
	unsigned int badFramesCounter = 0; // debug
	unsigned int totalCouldNotMatchCounter = 0; // debug
	unsigned int newNodeCounter = 0; // debug

//	AsusProLive::start();
	AsusProLiveOpenNI2::start();
	
	// grab some images until brightness is adjusted correctly
	
	for(int i=0; i < 5; ++i)
	{
		if(!AsusProLiveOpenNI2::grab(frames[state]))
		{
			--i;
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
	}

	// wait until backend sets the starting position
	while(!backend.currentPosUpToDate()) boost::this_thread::sleep(boost::posix_time::milliseconds(1));

	// first frame
	bool noError = false;
	while(backend.running() && AsusProLiveOpenNI2::running())
	{

    	noError = AsusProLiveOpenNI2::grab(frames[state]);

		if(!noError)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			continue;
		}
		else
			++totalNumberOfFrames; // debug

		if(!VisualOdometry::setKeypoints(frames[state]))
		{
			cout << "Bad Frame Nr. " << ++badFramesCounter << endl; // debug
			continue;
		}

		tmCurToNode = Eigen::Matrix4f::Identity();
		imCurToNode = Matrix<float, 6, 6>::Identity(); // value not needed
		backend.setNewNode(frames[state],tmCurToNode,imCurToNode,1);
		state = state?0:1; // flip state  
		break;
	}

	newNodeCounter = 1;

	
	// process
	double start, end, dif; // debug
	while(backend.running() && AsusProLiveOpenNI2::running() && ros::ok())
	{
		
		//debug
		start = static_cast<double>( clock () ) /  CLOCKS_PER_SEC;
		
		bool newNode;
		noError = AsusProLiveOpenNI2::grab(frames[state]);

		if(!noError){
        	boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        	continue;
        	}
		else
			++totalNumberOfFrames; // debug
		
		
		// TODO debug
		end = static_cast<double>( clock () ) /  CLOCKS_PER_SEC; //debug
		dif = (end - start)*1000.0; // note down grabbing time 

		//cout << "Grabbing took " << dif << "ms" << endl; //debug

		start = static_cast<double>( clock () ) /  CLOCKS_PER_SEC; //debug
			
		if(!VisualOdometry::setKeypoints(frames[state])){	
			cout << "Bad Frame Nr. " << ++badFramesCounter << endl; 
			cout << "Compensate with LPE instead" << endl; 

			tmCurToNode = logger.getLpe() * currentPos.inverse();	// get edge from LPE
 
			if ((logger.getTime() - currentTime)> 100) 		// initialization
			{	
				currentTime = logger.getTime();
				currentPos  = logger.getLpe();
			}
				
			// check delta movement
			if ((logger.getTime() != 0) && (!VisualOdometry::checkReliability(tmCurToNode,logger.getTime() - currentTime))) {		
			cout << "[LPE] time diff " <<(logger.getTime() - currentTime) << endl;
			cout << "[LPE] transform" << endl <<  tmCurToNode << endl;
				
			imCurToNode = Matrix<float, 6, 6>::Identity() * 100000; 	// experimental (need adjustable?) value for LPE measurement
			backend.setNewNode(frames[state],tmCurToNode,imCurToNode,1);
			cout << "[LPE] Node " << ++newNodeCounter << " sent to backend" << endl; //debug
			
			// note down last node in case no visual estimate in between 
			currentTime = logger.getTime(); 
			currentPos  = logger.getLpe(); 	
			state = state ? 0 : 1;	
			}

			continue;
		}



		// process frame: with current frame to last frame
		if(processFrame(frames[state],frames[state?0:1],tmCurToNode,imCurToNode,newNode))
		{
			couldNotMatchCounter = 0;
			cameraPos = backend.getCurrentPosition();

			// fuse data with IMU TODO

			// send position to UAV TODO

			// set new node
			if(newNode)
			{	
				
				cout << endl << "Node " << ++newNodeCounter << " sent to backend" << endl; // debug	
				cout << "visual slam estimate" << endl;							
				logger.updatePos(cameraPos);
				backend.setNewNode(frames[state],tmCurToNode,imCurToNode,1);

				//when image kicks in at anytime, we want edge from that point. (note this relative approach) 				 
				currentTime = logger.getTime(); 
				currentPos  = logger.getLpe();

				state = state ? 0 : 1;
			}
		}
		else // leave this to be dummy ? or exchange with IMU
		{
			cout << "Could not match current frame!" << endl; // debug
			++totalCouldNotMatchCounter; // debug

			if( ++couldNotMatchCounter >= setNewNodeAfterNrOfFailedMatches) // after certain threshold, send back a dummy frame (with nearly no 
			{
				cout << "Dummy node " << ++dummyFrameCounter << " sent to backend" << endl; // debug
				// dummy transformation, TODO exchange with IMU estimate
				
				tmCurToNode = Eigen::Matrix4f::Identity();
				imCurToNode = Matrix<float, 6, 6>::Identity() * 1e-100;

				backend.setNewNode(frames[state],tmCurToNode,imCurToNode,0);
				state = state?0:1;
			}
		}

		//
		// TODO debug
		end = static_cast<double>( clock () ) /  CLOCKS_PER_SEC; //debug
		dif = (end - start)*1000.0;
		ros::spinOnce();
		//cout << "Frontend processing took " << dif << "ms" << endl; //debug
	}

//	AsusProLive::stop();
	AsusProLiveOpenNI2::stop();

	// debug stats
	cout << "totalNumberOfFrames = " << totalNumberOfFrames << endl; // debug
	cout << "newNodeCounter = " << newNodeCounter << endl; // debug
	cout << "dummyFrameCounter = " << dummyFrameCounter << endl; // debug
	cout << "badFramesCounter = " << badFramesCounter << endl; // debug
	cout << "totalCouldNotMatchCounter = " << totalCouldNotMatchCounter << endl; // debug

	while(backend.running()) boost::this_thread::sleep(boost::posix_time::milliseconds(100)); //TODO debug

	return 0;
}

#endif
