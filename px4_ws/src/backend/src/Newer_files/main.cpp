/**
 * @file main.cpp
 * @author Gian Danuser & Michael Eugster
 * @brief This file contains the main function of the back end project.
 *
 */

#include <iostream>

#include "opencv2/highgui.hpp" // debug

#include "Frontend.h"
#include "Mapping.h"
#include "OrbDetSurfDesc.h"
#include "SURF.h"
#include "SIFT.h"
#include "PointCloudMap.h"
#include "RANSACBasedTME.h"
#include "G2oPoseGraph.h"

using namespace std;
using namespace SLAM;

//
// settings
//
static constexpr char frontEndIp[] = "192.168.144.11"; // WLAN

enum {
	frontEndPortNr = 11000, ///< port of the front end
	minMatches = 17, ///< minimal number of matches
	maxNrOfFeatures = 600, ///< maximal number of features to detect (only needed for OrbDetSurfDesc)
	sufficientMatches = 600, ///< sufficient number of matches to return
	maxRansacIterations = 1000, ///< maximal number of RANSAC iterations
	minNrOfInlier = 10 ///< minimal number of inlier for the RANSAC
};
static constexpr float maxDistanceForInlier = 0.02f; ///< maximal distance for inlier in meter (RANSAC)
static constexpr float thresholdAbsolutDistanceTest = 0.03f; ///< threshold for the absolute distance test in meter (RANSAC)
static constexpr float sufficentPercentageOfInlier = 0.6f; ///< percentage of inlier to terminate the RANSAC
static constexpr float descriptorRatio = 0.95f; ///< feature match ratio
static constexpr float voxelSize = 0.02f; ///< voxel grid size


//
// static objects
//
static SURF fdem(descriptorRatio, minMatches, sufficientMatches);
//static OrbDetSurfDesc fdem(descriptorRatio, maxNrOfFeatures, minMatches, sufficientMatches);
//static SIFT fdem(descriptorRatio, minMatches, sufficientMatches);
static RANSACBasedTME tme(maxRansacIterations, maxDistanceForInlier, thresholdAbsolutDistanceTest, sufficentPercentageOfInlier, minNrOfInlier);
static G2oPoseGraph graph;
static PointCloudMap pointCloudMap(voxelSize);

//
// log position
//
static std::ofstream logPos;
static void logPoseGraphNode(const Frame& frame, const Eigen::Isometry3d& pose)
{
	Eigen::Affine3d trafo(pose.affine());
	Eigen::Quaterniond rotation(trafo.rotation());
	logPos << to_string(frame.getTime()) << " "
		 << pose.translation().x() << " "
		 << pose.translation().y() << " "
		 << pose.translation().z() << " "
		 << rotation.x() << " "
		 << rotation.y() << " "
		 << rotation.z() << " "
		 << rotation.w() << endl;
}

int main()
{
	//
	// init
	Frame frame;
	Eigen::Isometry3d tm;
	Eigen::Matrix<double,6,6> im;

	//
	// setup backend
	Mapping slam(&fdem, &tme, &graph, &pointCloudMap);

	//
	// start frontend
	Frontend frontend(frontEndIp, frontEndPortNr);

	//
	// start viewer and logger
	logPos.open("position.txt", std::ofstream::out | std::ofstream::trunc);
	pointCloudMap.startMapViewer();

	//
	// set start position on front end
	frontend.setCurrentPosition(slam.getCurrentPosition());

	//
	// process frames
	bool stop = false;
	cout << "before loop" << endl;
	while(frontend.running() && !stop)
	{
		int toNode = frontend.getNewNode(frame, tm, im);
		if(toNode > -1)
		{
			// imshow
			cv::imshow("RGB Image", frame.getRgb());
//			cv::imshow("Gray Image", frame.getGray());
			const float scaleFactor = 0.05f;
			cv::Mat depthMap;
			frame.getDepth().convertTo( depthMap, CV_8UC1, scaleFactor );
			cv::imshow("Depth Image", depthMap);
			stop |= (cv::waitKey(10) >= 0); //stop when key pressed

			//
			// start slam
			slam.addFrame(frame);
			slam.run();
			//frontend.setCurrentPosition(slam.getCurrentPosition()); //
			//logPoseGraphNode(frame, slam.getCurrentPosition());	//
			pointCloudMap.updateMapViewer();
			cout << "inside if" << endl;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
	}

//	graph.optimize();
//	graph.removeEdgesWithErrorBiggerThen(10.0/(0.02*0.02));
	graph.optimizeTillConvergenz();

	pointCloudMap.updateMapViewer();

	logPos.close();
	pointCloudMap.saveMap("/home/dexheimere/MAV-Project/px4_ws/src/backend/Map.pcd");	// change this with full directory name


	// plot statistic
	cout << "Total number of Frames " << slam.getFrameCounter() << endl;
	cout << "Number of bad frames " << slam.getBadFrameCounter() << endl;
	cout << "Number of frames where no transformation was found " << slam.getNoTrafoFoundCounter() << endl;
	cout << "Number of frames where replaced by a dummy node " << slam.getDummyNodeCounter() << endl;
	cout << "Number of loop closures found " << slam.getDetLoopClsrsCounter() << endl;
	cout << "Number of too small transformations " << slam.getTrafoToSmallCounter() << endl;
	cout << "Number of transformations with too big velocities " << slam.getTrafoVelocityToBigCounter() << endl;
	cout << "Number of key frames " << slam.getKeyFrames().size() << endl;
	cout << "Number of nodes " << slam.getNodes().size() << endl << endl;
	cout << "Current graph Id " << slam.getNodes().back().getId() << endl << endl;

	pointCloudMap.showMap();
	pointCloudMap.stopMapViewer();

	return 0;
}
