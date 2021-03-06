/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2018, PickNik LLC
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of PickNik LLC nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Henning Kayser
 * Desc: A planning interface that offers convenient and thread-safe collision checking and planning with RapidPlan.
 */

#ifndef RTR_MOVEIT_RTR_PLANNER_INTERFACE_H
#define RTR_MOVEIT_RTR_PLANNER_INTERFACE_H

// C++
#include <deque>
#include <map>
#include <mutex>
#include <string>
#include <vector>

// ROS
#include <ros/ros.h>

// MoveIt!
#include <moveit/macros/class_forward.h>

// RapidPlan
#include <rtr-api/MPAInterface.hpp>
#include <rtr-api/PathPlanner.hpp>
#include <rtr-api/RapidPlanDataTypes.hpp>
#include <rtr-occupancy/Voxel.hpp>

// rtr_moveit
#include <rtr_moveit/rtr_datatypes.h>

namespace rtr_moveit
{
MOVEIT_CLASS_FORWARD(RTRPlannerInterface);

// A RapidPlan goal specification
struct RapidPlanGoal
{
  // RapidPlan supports either ids of roadmap states or tool pose transforms as goals
  enum Type
  {
    STATE_IDS,
    TOOL_POSE,
  };
  Type type;

  // STATE_IDS: a list of target states in the roadmap
  std::vector<std::size_t> state_ids;

  // TOOL_POSE an endeffector transform to look for a target state
  rtr::ToolPose tool_pose;
  rtr::ToolPose tolerance;  // pose tolerance of the target state
  rtr::ToolPose weights;    // pose distance weights for ranking multiple solutions
};
class RTRPlannerInterface
{
public:
  RTRPlannerInterface(const ros::NodeHandle& nh);
  virtual ~RTRPlannerInterface();

  /** \brief Initialize the RapidPlanInterface */
  bool initialize();

  /** \brief Check if the RapidPlanInterface is available and the planner can receive requests */
  bool isReady() const;

  /** \brief Run planning attempt and generate a solution path */
  bool solve(const RoadmapSpecification& roadmap_spec, const std::size_t start_state_id, const RapidPlanGoal& goal,
             const OccupancyData& occupancy_data, const double& timeout, std::vector<rtr::Config>& solution_path);

  /** \brief Run planning attempt and generate solution waypoints and edges */
  bool solve(const RoadmapSpecification& roadmap_spec, const std::size_t start_state_id, const RapidPlanGoal& goal,
             const OccupancyData& occupancy_data, const double& timeout, std::vector<rtr::Config>& roadmap_states,
             std::deque<std::size_t>& waypoints, std::deque<std::size_t>& edges);

  /** \brief Get the configs of the given roadmap */
  bool getRoadmapConfigs(const RoadmapSpecification& roadmap_spec, std::vector<rtr::Config>& configs);

  /** \brief Get the edges of the given roadmap */
  bool getRoadmapEdges(const RoadmapSpecification& roadmap_spec, std::vector<rtr::Edge>& edges);

  /** \brief Get the tool transforms of the given roadmap */
  bool getRoadmapTransforms(const RoadmapSpecification& roadmap_spec, std::vector<rtr::ToolPose>& transforms);

private:
  /** \brief load roadmap file to PathPlanner and store roadmap specification */
  bool loadRoadmapToPathPlanner(const RoadmapSpecification& roadmap_spec);

  /** \brief Initialize PathPlanner and RapidPlanInterface with a given roadmap identifier */
  bool prepareRoadmap(const RoadmapSpecification& roadmap_spec, size_t& roadmap_index);

  /** \brief Find the roadmap index for a given roadmap name */
  bool findRoadmapIndex(const std::string& roadmap_name, size_t& roadmap_index)
  {
    for (auto it = roadmap_indices_.begin(); it != roadmap_indices_.end(); it++)
    {
      if (it->second == roadmap_name)
      {
        roadmap_index = it->first;
        return true;
      }
    }
    return false;
  }

  ros::NodeHandle nh_;
  bool debug_ = false;

  // mutex lock for thread-safe RapidPlan calls
  std::mutex mutex_;

  // RapidPlan interfaces
  rtr::MPAInterface rapidplan_interface_;
  rtr::PathPlanner planner_;
  bool rapidplan_interface_enabled_ = true;

  // available roadmap specifications
  std::map<std::string, RoadmapSpecification> roadmaps_;
  // name of roadmap loaded by the planner
  std::string loaded_roadmap_;
  // indices of roadmaps written to the board
  std::map<uint16_t, std::string> roadmap_indices_;
};
}  // namespace rtr_moveit

#endif  // RTR_MOVEIT_RTR_PLANNER_INTERFACE_H
