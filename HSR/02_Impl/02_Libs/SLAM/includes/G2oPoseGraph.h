 /**
 * @file G2oPoseGraph.h
 * @author Gian Danuser & Michael Eugster
 * @brief This file contains the declaration of the G2oPoseGraph class
 *
 */
#ifndef G2OPOSEGRAPH_H_
#define G2OPOSEGRAPH_H_

#include <vector>

#include <boost/thread/mutex.hpp>

#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/block_solver.h"
#include "g2o/core/optimization_algorithm_levenberg.h"
#include "g2o/core/optimization_algorithm_gauss_newton.h"
#include "g2o/core/optimization_algorithm_dogleg.h"
#include "g2o/solvers/csparse/linear_solver_csparse.h"
#include "g2o/types/slam3d/types_slam3d.h"

#include "IPoseGraph.h"

namespace SLAM {

/**
 * @class G2oPoseGraph G2oPoseGraph.h "G2oPoseGraph.h"
 * @brief The G2oPoseGraph build a pose graph using the g²o frame work
 */
class G2oPoseGraph : public IPoseGraph {
public:
	/**
	 * @brief Constructor
	 */
	G2oPoseGraph();

	/**
	 * @brief Destructor
	 */
	virtual ~G2oPoseGraph();

	//
	// see IGraphOptimizer.h for description
	virtual int getCurrentId() { return currentIndex; }
	virtual int addFirstNode();
	virtual int addNode(const Eigen::Isometry3d& pos);
	virtual void removeNode(int id);
	virtual void addEdgePreviousToCurrent(const Eigen::Isometry3d& measurement, const Eigen::Matrix<double, 6, 6>& informationMatrix);
	virtual void addEdgeFromIdToCurrent(const Eigen::Isometry3d& measurement, const Eigen::Matrix<double, 6, 6>& informationMatrix, int idFrom);
	virtual void addEdgeFromCurrentToId(const Eigen::Isometry3d& measurement, const Eigen::Matrix<double, 6, 6>& informationMatrix, int idTo);
	virtual void addEdgeFromIdToId(const Eigen::Isometry3d& measurement, const Eigen::Matrix<double, 6, 6>& informationMatrix, int idFrom, int idTo);
	virtual bool removeEdgesWithErrorBiggerThen(double threshold);
	virtual void optimize();
	virtual void optimizeTillConvergenz();
	virtual const Eigen::Isometry3d& getPositionOfId(int id);
	virtual const Eigen::Isometry3d& getCurrentPosition() { return getPositionOfId(currentIndex); }
	virtual void getEdgeNeighborsToCurrentNode(int sequential, std::vector<int>& neighborIds);

	/**
	 * @brief saves the pose graph
	 */
	void save() { optimizer.save("graph.g2o"); }

private:
	// allocating the optimizer
	g2o::LinearSolverCSparse<g2o::BlockSolver< g2o::BlockSolverTraits<-1, -1> >::PoseMatrixType> linearSolver;
	g2o::BlockSolver< g2o::BlockSolverTraits<-1, -1> > blockSolver;
//	g2o::OptimizationAlgorithmGaussNewton solver;
	g2o::OptimizationAlgorithmLevenberg solver;
//	g2o::OptimizationAlgorithmDogleg solver;
	g2o::SparseOptimizer optimizer;
	enum{ optMaxIter = 1 };
	const double optConvergence = 0.999;

	int currentIndex;

	boost::mutex graphMutex;

};

} /* namespace SLAM */

#endif /* G2OPOSEGRAPH_H_ */
