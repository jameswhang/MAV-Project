 /**
 * @file Mapping.h
 * @author Gian Danuser & Michael Eugster
 * @brief This file contains the declaration of the Mapping class
 *
 */
#ifndef MAPPING_H_
#define MAPPING_H_

#include <string>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <IPoseGraph.h>

#include <pcl/console/time.h>   // TicToc

#include "IFeatures.h"
#include "ITransformMatEst.h"
#include "IMap.h"
#include "Frame.h"
#include "FrameToPcConverter.h"

namespace SLAM {

/**
 * @class Mapping Mapping.h "Mapping.h"
 * @brief The Mapping class is the main class of the SLAM process and is responsible for the scheduling of the tasks
 */
class Mapping {
public:
	/**
	 * @brief printPosition prints a pose in matrix form
	 *
	 * @param position pose (in)
	 */
	static void printPosition(const Eigen::Isometry3d& position);

	/**
	 * @brief Constructor
	 *
	 * @param aFDEM a feauter detector, extractor and matcher (in)
	 * @param aTME a transforamtion matrix estimator (in)
	 * @param aGO a pose graph (in)
	 * @param aMap a map (in)
	 */
	Mapping(
			IFeatures* aFDEM,
			ITransformMatEst* aTME,
			IPoseGraph* aGO,
			IMap* aMap );

	/**
	 * @brief Destructor
	 */
	virtual ~Mapping();

	/**
	 * @brief processes the new frame
	 */
	void run();

	/**
	 * @brief adds a new frame
	 *
	 * @param frame new frame (in)
	 */
	void addFrame(Frame& frame);

// in thread and execute asynchronously
	void addNewNode();
	void optPoseGraph();

	/**
	 * @brief returns the current position
	 *
	 * @return current position
	 */
	const Eigen::Isometry3d& getCurrentPosition() { return currentPosition; }

	//
	// debug
	unsigned int getFrameCounter() {return frameCounter;}
	unsigned int getBadFrameCounter() {return badFrameCounter;}
	unsigned int getNoTrafoFoundCounter() {return noTrafoFoundCounter;}
	unsigned int getTrafoToSmallCounter() {return trafoToSmallCounter;}
	unsigned int getDetLoopClsrsCounter() {return detLoopClsrsCounter;}
	unsigned int getTrafoVelocityToBigCounter() {return trafoVelocityToBigCounter;}
	unsigned int getDummyNodeCounter() {return dummyNodeCounter;}

	const std::vector<Frame>& getNodes() {return nodes;}
	const std::vector<Frame>& getKeyFrames() {return keyFrames;}


	enum{ contFramesToMatch = 3, ///< number of sequential frames to match
		  neighborsToMatch = 3, ///< number of neighbor frames to match
		  minNumberOfKeyPoints = 30, ///< min number of key points
		  dummyFrameAfterLostFrames = 5 ///< number of frames, which cannot be matched until a dummy frame is created
		};
	static constexpr bool onlineOptimization = true; ///< Enables/disable online optimization
	static constexpr bool optimizeTillConvergence = false; ///< Enables/disable optimization till convergence
	static constexpr bool addDummyNodeFlag = true; ///< Enables/disable dummy nodes
	static constexpr bool exchangeFirstNode = false; ///< Enables/disable exchanges first node
	static constexpr bool searchLoopClosures = true; ///< Enables/disable loop closure search
	static constexpr int lcRandomMatching = 0; ///< Enables/disable random loop closure matching 0 --> off (use average descriptor), else match n random key frames
	static constexpr bool removeEdgesWithBigErrors = false; ///< Enables/disable remove edeges with big errors
	// static constexpr double minRotation = -1.0;  ///< minimal rotation in rad(negative values to disable)
	// static constexpr double minTranslation = -1.0;  ///< minimal translation  in meter(negative values to disable)
	static constexpr double minRotation = 1.0*M_PI/180.0;  ///< minimal rotation in rad(negative values to disable)
	static constexpr double minTranslation = 0.01;   ///< minimal translation in meter(negative values to disable)
	static constexpr double maxVelocity = std::numeric_limits<double>::infinity(); ///< max velocity in meter per second
	static constexpr double maxAngularVelocity = std::numeric_limits<double>::infinity(); ///< max angular velocity in rad per second
//	static constexpr double maxVelocity = 5.0; ///< max velocity in meter per second
//	static constexpr double maxAngularVelocity = 90.0 *M_PI / 180.0; ///< max angular velocity in rad per second
	static constexpr double loopClosureDetectionThreshold = std::numeric_limits<double>::infinity(); ///< loop closure detection threshold for the averge descriptor
	static constexpr double edgeErrorThreshold = 30.0 / (0.02 * 0.02); ///< g²o threshold for eges with big errors

private:

	//
	// helper functions
	//
	bool featureDetectionAndExtraction();
	void initialMatching();
	void exchangeFirstFrame();
	void parallelMatching(Frame procFrame);
	void tryToAddNode(int thread);
	void addEdges(int thread,int currId);
	void setDummyNode();
	bool searchKeyFrames(Frame procFrame);
	void optimizeGraph(bool tillConvergenz);
	void updateMap();
	void matchTwoFrames(
				const Frame& frame1, // in: new frame
				const Frame& frame2, // in: older frames
				bool& enoughMatches, // out
				bool& validTrafo,// out
				Eigen::Isometry3d& transformationMatrix, // out: transformation from frame 2 to frame 1
				Eigen::Matrix<double, 6, 6>& informationMatrix  // out: information matrix of the estimation
				);
	enum GraphProcessingResult {trafoToSmall, trafoToBig, trafoValid};
	GraphProcessingResult processGraph(const Eigen::Isometry3d& transformationMatrix, const Eigen::Matrix<double, 6, 6>& informationMatrix, \
		 int prevId, int currId,  double deltaTime, bool tryToAddNode, bool possibleLoopClosure);
	inline void convertRotMatToEulerAngles(const Eigen::Matrix3d& t, double& roll, double& pitch, double& yaw) const;
	bool isMovementBigEnough(const Eigen::Isometry3d& trafo) const;
	bool isVelocitySmallEnough(const Eigen::Isometry3d& trafo, double deltaT) const;
	void addFirstNode();
	enum ComparisonResult {noTimeStamp, succeed, failed};
	ComparisonResult compareCurrentPosition(const Frame& frame, const Eigen::Isometry3d& pose);
	void loopClosureDetection(Frame procFrame);

	//
	// variables
	//
	IFeatures* fdem;
	ITransformMatEst* tme;
	IPoseGraph* poseGraph;
	IMap* map3d;

	volatile bool initDone = false;
	volatile bool loopClosureFound = false;

	double bestInforamtionValue = 0;
	int smallestId = 0;
	int frames = 0;

	Eigen::Isometry3d currentPosition;
	Frame currentFrame;

	std::vector<Frame> nodes;
	std::vector<Frame> keyFrames;

	// threads
	boost::thread handler[lcRandomMatching + contFramesToMatch + neighborsToMatch];
  boost::thread graphHandler;
	bool optFlag = true;

	bool enoughMatches[lcRandomMatching + contFramesToMatch + neighborsToMatch];
	bool validTrafo[lcRandomMatching + contFramesToMatch + neighborsToMatch];
	Eigen::Isometry3d transformationMatrices[lcRandomMatching + contFramesToMatch + neighborsToMatch];
	Eigen::Matrix<double, 6, 6> informationMatrices[lcRandomMatching + contFramesToMatch + neighborsToMatch];
	int graphIds[lcRandomMatching + contFramesToMatch + neighborsToMatch];
	double deltaT[lcRandomMatching + contFramesToMatch + neighborsToMatch];

	int lcSmallestId;
	boost::thread lcHandler;
	Eigen::Isometry3d lcTm;
	Eigen::Matrix<double, 6, 6> lcIm;
	// this really need initialization... folks...
	bool lcEnoughMatches = 0;
	bool lcValidTrafo = 0;
	int lcBestIndex = -1;

	boost::mutex frameUpdateMutex;

	int sequenceOfLostFramesCntr = 0;

	// debugging
	pcl::console::TicToc time;
	pcl::console::TicToc time_delay;
	unsigned int frameCounter = 0;
	unsigned int badFrameCounter = 0;
	unsigned int noTrafoFoundCounter = 0;
	unsigned int trafoToSmallCounter = 0;
	unsigned int detLoopClsrsCounter = 0;
	unsigned int trafoVelocityToBigCounter = 0;
	unsigned int dummyNodeCounter = 0;

	pcl::console::TicToc optTime;
	double optAvrTime = 0;
	double optMaxTime = 0;
	unsigned int nOpt = 0;

	double frameProcMeanTime = 0;
	double frameProcMaxTime = 0;
	int nframeProc = 0;

public:
	double getOptAvrTime() { return optAvrTime / static_cast<double>(nOpt); }
	double getOptMaxTime() { return optMaxTime; }

	double getFrameProcMeanTime() {return frameProcMeanTime / static_cast<double>(nframeProc);};
	double getFrameProcMaxTime() {return frameProcMaxTime;};
};

} /* namespace SLAM */

#endif /* MAPPING_H_ */
