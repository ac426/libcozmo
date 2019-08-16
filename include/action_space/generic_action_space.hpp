////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019,  Brian Lee, Vinitha Ranganeni
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

#ifndef LIBCOZMO_ACTIONSPACE_GENERICACTIONSPACE_HPP
#define LIBCOZMO_ACTIONSPACE_GENERICACTIONSPACE_HPP

#include <ros/ros.h>
#include <libcozmo/ActionMsg.h>
#include <Eigen/Dense>
#include <cmath>
#include "utils/utils.hpp"
#include "ActionSpace.hpp"

namespace libcozmo {
namespace actionspace {

/// This class represents the generic actionspace for Cozmo
/// Actions are generated with respect to Cozmo's position at any given time
/// Possible actions are generated from given options for speed, and duration
/// Possible headings are calculated by defining division of 2PI angle into 
/// given number of headings to generate. e.g. number of headings defined as 4
/// generate 4 possible headings: 0, 90, 180, 270 (degrees), otherwise seen as
/// "right, front, left, back" when plotted
/// Hence heading is calculated with respect of x,y axis on 2D representation.
/// The total number of possible actions then equals to
/// = number of possible speed * number of possible duration * number of
/// possible heading.
class GenericActionSpace : public virtual ActionSpace {
 public:
    /// Generic Action class
    class Action : public ActionSpace::Action {
     public:
        /// Constructor

        /// \param speed The speed of action (m/s)
        /// \param duration The duration of action (s)
        /// \param heading The heading of action (rad)
        explicit Action(
            const double& speed,
            const double& duration,
            const double& heading) : \
            m_speed(speed),
            m_duration(duration),
            m_heading(heading) {}

        /// Speed of action (m/s)
        const double m_speed;

        /// Duration of action (s)
        const double m_duration;

        /// Heading of action (rad)
        const double m_heading;
    };
    /// Constructor

    /// \param m_speeds Vector of available speeds
    /// \param m_durations Vector of available durations
    /// \param num heading Number of options for heading/direction (required:
    /// power of 2 and >= 4)
    GenericActionSpace(
        const std::vector<double>& m_speeds,
        std::vector<double> m_durations,
        const int& num_heading) {
            int num_speed = m_speeds.size();
            int num_duration = m_durations.size();
            std::vector<double> m_headings = utils::linspace(
                0.0,
                2.0 * M_PI - 2.0 * M_PI / num_heading,
                num_heading);
            m_actions = std::vector<Action*>(
                num_speed *
                num_duration *
                num_heading,
                nullptr);
            for (int j = 0; j < num_speed; j++) {
                for (int k = 0; k < num_duration; k++) {
                    for (int l = 0; l < num_heading; l++) {
                        const int id =
                            j * num_duration * num_heading +
                            k * num_heading + l;
                        m_actions[id] = new Action(
                            m_speeds[j],
                            m_durations[k],
                            m_headings[l]);
                  }
                }
            }
        }
    ~GenericActionSpace() {
        for (int i = 0; i < m_actions.size(); ++i) {
            delete(m_actions[i]);
        }
        m_actions.clear();
    }

    /// Calculates similarity between two actions
    /// Similarity based on the Euclidean distance between the actions

    /// \param action_id1, actionid2 IDs of actions to compare
    /// \return similarity
    bool action_similarity(
        const int& action_id1,
        const int& action_id2,
        double* similarity) const override;

    /// Documentation inherited
    ActionSpace::Action* get_action(const int& action_id) const override;

    /// Documentation inherited
    bool publish_action(
        const int& action_id,
        const ros::Publisher& publisher) const override;

    /// Documentation inherited
    bool is_valid_action_id(const int& action_id) const override;

    /// Documentation inherited
    int size() const override;

 private:
    /// Vector of actions
    std::vector<Action*> m_actions;
};
}  /// namespace actionspace
}  /// namespace libcozmo

#endif
