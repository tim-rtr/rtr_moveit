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
 * Desc: henningkayser@picknik.ai
 */

#ifndef RTR_MOVEIT_RTR_PLANNING_CONTEXT_H
#define RTR_MOVEIT_RTR_PLANNING_CONTEXT_H

#include <string>

#include <moveit/macros/class_forward.h>
#include <moveit/planning_interface/planning_interface.h>
#include <rtr_moveit/rtr_planner_interface.h>

namespace rtr_moveit
{
MOVEIT_CLASS_FORWARD(RTRPlanningContext);
MOVEIT_CLASS_FORWARD(RTRPlannerInterface);

class RTRPlanningContext : public planning_interface::PlanningContext
{
public:
  RTRPlanningContext(const std::string& name, const std::string& group,
                     const RTRPlannerInterfacePtr& planner_interface);

  virtual ~RTRPlanningContext()
  {
  }

  virtual bool solve(planning_interface::MotionPlanResponse& res);
  virtual bool solve(planning_interface::MotionPlanDetailedResponse& res);

  virtual void clear();
  virtual bool terminate();

private:
  const RTRPlannerInterfacePtr planner_interface_;
};
}  // namespace rtr_moveit

#endif  // RTR_MOVEIT_RTR_PLANNING_CONTEXT_H