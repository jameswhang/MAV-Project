/**
 * @file main.cpp
 * @author Gian Danuser & Michael Eugster
 * @brief This file contains the front end main loop.
 *
 */
#include <iostream>
#include <ctime>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "VisualOdometry.h"
//#include "AsusProLive.h"
#include "AsusProLiveOpenNI2.h"
#include "Backend.h"

using namespace std;
using namespace Eigen;


//
// Settings
//
enum {
	setNewNodeAfterNrOfFailedMatches = 1
};
static constexpr int backendPort = 11000;

/**
 * @brief Process the new frame (estimate the transformation)
 *
 * @param newFrame the new captured frame (input)
 * @param prevNode the previous node (input)
 * @param tmCurToNode transformation matrix from the current frame to the node (output)
 * @param imCurToNode information matrix from the current frame to the node (output)
 * @param newNode true if the newFrame is a new node, false otherwise (output)
 *
 * @return Returns true if there is a valid transformation between the newFrame and the prevNode, false otherwise.
 */
static bool processFrame(const Frame& newFrame, const Frame& prevNode, Matrix4f& tmCurToNode, Matrix<float, 6, 6>& imCurToNode, bool& newNode)
{
	//
	// estimate transforamtion
	bool validFlag;
	VisualOdometry::estimateTrafo(newFrame, prevNode, tmCurToNode, imCurToNode, validFlag);

	//
	// process result
	VisualOdometry::Result res;
	if (validFlag)
	{
		res = VisualOdometry::checkReliability(tmCurToNode, newFrame.getTime() - prevNode.getTime());
		validFlag = (res != VisualOdometry::invalid);
	}

	if (validFlag)
	{
		newNode = (res == VisualOdometry::valid);
		return true;
	}
	else
	{
		newNode = false;
		return false;
	}
}

enum{newNode = 0, prevNode = 1};

/**
 * @brief main of the front end
 */
int main()
{
	//
	// initialization
	//
	int state = newNode;
	Frame frames[2];
	Matrix4f tmCurToNode;
	Matrix<float, 6, 6> imCurToNode;

	boost::mutex backendMutex;
	Backend backend(backendPort, backendMutex);
	unsigned int couldNotMatchCounter = 0;

	unsigned int totalNumberOfFrames = 0; // debug
	unsigned int dummyFrameCounter = 0; // debug
	unsigned int badFramesCounter = 0; // debug
	unsigned int totalCouldNotMatchCounter = 0; // debug
	unsigned int newNodeCounter = 0; // debug

	//
	// start the RGBD sensor
//	AsusProLive::start();
	AsusProLiveOpenNI2::start();

	//
	// grab some images until brightness is adjusted correctly
	for(int i=0; i < 5; ++i)
	{
//		AsusProLive::grab(frames[state].setRgb(), frames[state].setGray(), frames[state].setDepth(), frames[state].setTime());
		AsusProLiveOpenNI2::grab(frames[state]);
	}

	//
	// wait until backend sets the starting position
	while(!backend.currentPosUpToDate()) boost::this_thread::sleep(boost::posix_time::milliseconds(1));

	//
	// first frame
	bool noError = false;
	while(backend.running() && AsusProLiveOpenNI2::running()  && !noError)
	{
//		noError = AsusProLive::grab(frames[state].setRgb(), frames[state].setGray(), frames[state].setDepth(), frames[state].setTime());
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
			noError = false;
			continue;
		}

		tmCurToNode = Eigen::Matrix4f::Identity();
		imCurToNode = Matrix<float, 6, 6>::Identity(); // value not needed
		backend.setNewNode(frames[state],tmCurToNode,imCurToNode,1);
		state = state?0:1;
	}

	newNodeCounter = 1;

	//
	// process
	double start, end, dif; // debug
	while(backend.running() && AsusProLiveOpenNI2::running())
	{
		start = static_cast<double>( clock () ) /  CLOCKS_PER_SEC; //debug

		bool newNode;
//		noError = AsusProLive::grab(frames[state].setRgb(), frames[state].setGray(), frames[state].setDepth(), frames[state].setTime());
		noError = AsusProLiveOpenNI2::grab(frames[state]);

		if(!noError)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			continue;
		}
		else
			++totalNumberOfFrames; // debug

		//
		// TODO debug
		end = static_cast<double>( clock () ) /  CLOCKS_PER_SEC; //debug
		dif = (end - start)*1000.0;
		cout << "Grabbing took " << dif << "ms" << endl; //debug


		start = static_cast<double>( clock () ) /  CLOCKS_PER_SEC; //debug

		if(!VisualOdometry::setKeypoints(frames[state]))
		{
			cout << "Bad Frame Nr. " << ++badFramesCounter << endl; // debug
			continue;
		}

		if(processFrame(frames[state],frames[state?0:1],tmCurToNode,imCurToNode,newNode))
		{
			couldNotMatchCounter = 0;

			//
			// fuse data with IMU TODO

			//
			// send position to UAV TODO

			//
			// set new node
			if(newNode)
			{
				cout << "Node " << ++newNodeCounter << " sent to backend" << endl; // debug

				backend.setNewNode(frames[state],tmCurToNode,imCurToNode,1);
				state = state?0:1;
			}
		}
		else
		{
			cout << "Could not match current frame!" << endl; // debug
			++totalCouldNotMatchCounter; // debug

			if( ++couldNotMatchCounter >= setNewNodeAfterNrOfFailedMatches)
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
		cout << "Frontend processing took " << dif << "ms" << endl; //debug
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


