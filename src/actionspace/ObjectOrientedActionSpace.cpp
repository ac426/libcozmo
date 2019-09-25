////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019, Brian Lee, Eric Pan, Vinitha Ranganeni
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     1. Redistributions of source code must retain the above copyright notice
//        this list of conditions and the following disclaimer.
//     2. Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//     3. Neither the name of the copyright holder nor the names of its
//        contributors may be used to endorse or promote products derived from
//        this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////

#include "actionspace/ObjectOrientedActionSpace.hpp"
#include "statespace/SE2.hpp"
#include "utils/utils.hpp"

namespace libcozmo {
namespace actionspace {

ObjectOrientedActionSpace::ObjectOrientedActionSpace(
    const std::vector<double>& speeds,
    const std::vector<double>& ratios,
    const double& edge_offset,
    const int& num_offset) : \
    m_speeds(speeds),
    m_ratios(ratios),
    num_offset(num_offset),
    m_center_offset(60),
    m_edge_offset(edge_offset) {
    std::vector<double> cube_offsets = num_offset == 1 ?
        std::vector<double>{0} :
        utils::linspace(-m_edge_offset, m_edge_offset, num_offset);
    int action_id = 0;
    const std::vector<double> sides{FRONT, LEFT, BACK, RIGHT};

    // Generate all possible generic actions given the heading offset, aspect
    // ratio, and speed
    for (const auto& heading_offset : sides) {
        const double ratio =
            (heading_offset == FRONT || heading_offset == BACK) ? 
                m_ratios[0] : m_ratios[1];
        for (const auto& cube_offset : cube_offsets) {
            for (const auto& speed : speeds) {
                m_actions.push_back(new GenericAction(
                    speed,
                    -cube_offset / m_edge_offset,
                    ratio,
                    heading_offset));
                action_id++;
            }
        }
    }
}

bool ObjectOrientedActionSpace::action_similarity(
    const int& action_id1,
    const int& action_id2,
    double* similarity) const {
    if (!(is_valid_action_id(action_id1) &&
          is_valid_action_id(action_id2))) {
        return false;
    }
    GenericAction* action1 = m_actions[action_id1];
    GenericAction* action2 = m_actions[action_id2];
    std::vector<double> action1_vector{
        action1->m_speed,
        action1->m_edge_offset,
        action1->m_aspect_ratio};
    std::vector<double> action2_vector{
        action2->m_speed,
        action2->m_edge_offset,
        action2->m_aspect_ratio};
    *similarity = utils::euclidean_distance(action1_vector, action2_vector);
    return true;
}

ActionSpace::Action* ObjectOrientedActionSpace::get_action(
    const int& action_id) const {
    if (!is_valid_action_id(action_id)) {
        return nullptr;
    }
    return m_actions[action_id];
}

bool ObjectOrientedActionSpace::is_valid_action_id(
    const int& action_id) const {
    return action_id < m_actions.size() && action_id >= 0;
}

bool ObjectOrientedActionSpace::get_generic_to_object_oriented_action(
    const int& action_id,
    const aikido::statespace::StateSpace::State& _state,
    ObjectOrientedAction* action) const {
    auto _action = static_cast<GenericAction*>(get_action(action_id));

    // handle segmentation fault in the case action is not valid
    if (_action == nullptr) {
        return false;
    }

    // Calculate action w.r.t position of the object
    auto state = static_cast<const aikido::statespace::SE2::State&>(_state);
    Eigen::Isometry2d transform = state.getIsometry();
    Eigen::Rotation2Dd rotation = Eigen::Rotation2Dd::Identity();
    rotation.fromRotationMatrix(transform.rotation());
    Eigen::Vector2d position = transform.translation();
    double angle = rotation.angle();
    double heading = _action->heading_offset();
    *action = ObjectOrientedAction(
        _action->speed(),
        Eigen::Vector3d(
            position[0] - m_center_offset * cos(heading) +
                _action->edge_offset() * m_edge_offset * sin(heading),
            position[1] - m_center_offset * sin(heading) +
                _action->edge_offset() * m_edge_offset * cos(heading),
            utils::angle_normalization(angle + heading)));

    return true;
}

bool ObjectOrientedActionSpace::publish_action(
    const int& action_id,
    const ros::Publisher& publisher,
    const aikido::statespace::StateSpace::State& _state) const {
    libcozmo::ObjectOrientedAction msg;
    ObjectOrientedAction OO_action(0.0, Eigen::Vector3d(0, 0, 0));
    if (!get_generic_to_object_oriented_action(action_id, _state, &OO_action)) {
        return false;
    }
    msg.speed = OO_action.speed();
    msg.duration = 1;
    Eigen::Vector3d start_pose = OO_action.start_pose();
    msg.x = start_pose[0];
    msg.y = start_pose[1];
    msg.theta = start_pose[2];
    publisher.publish(msg);
    return true;
}

int ObjectOrientedActionSpace::size() const {
    return m_actions.size();
}

}  // namespace actionspace
}  // namespace libcozmo
