/*
 * VisualOdometry.h
 *
 *  Created on: May 15, 2015
 *      Author: Gian Danuser & Michael Eugster
 */

#ifndef INCLUDES_VISUALODOMETRY_H_
#define INCLUDES_VISUALODOMETRY_H_

#include "Frame.h"

class VisualOdometry
{
public:
	static constexpr int minNrOfMatches = Frame::minNrOfMatches;
	static constexpr int minNrOfKeyPoints = Frame::minNrOfKeyPoints;

	static constexpr float minTranslation = Frame::minTranslation;
	static constexpr float minRotation = Frame::minRotation;
	static constexpr float maxTranslationPerSecond = Frame::maxTranslationPerSecond;
	static constexpr float maxAngularVelocit = Frame::maxAngularVelocit;

	static bool setKeypoints(Frame& frame);
	static void estimateTrafo(const Frame& srcFrame, const Frame& targetFrame,
				Eigen::Matrix4f& tmSrcToTarget, Eigen::Matrix<float, 6, 6>& imEstSrcToTarget, bool& valid);
	enum Result {valid, invalid, small};
	static Result checkReliability(const Eigen::Matrix4f& tm, const double& dt);

private:
	VisualOdometry() { } // no objects

	static constexpr float minSquaredTranslation = minTranslation*minTranslation; // m²
	static constexpr float maxSquaredTranslationPerSecond = maxTranslationPerSecond*maxTranslationPerSecond; // m²/s
};

#endif /* INCLUDES_VISUALODOMETRY_H_ */
