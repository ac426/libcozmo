#include <action_space/generic_action_space.hpp>
#include <exception>
#include <iostream>
namespace libcozmo {
namespace actionspace {

double GenericActionSpace::action_similarity(
    const int& action_id1,
    const int& action_id2) const {
    try {
        return sqrt(
            pow((m_actions[action_id1]->m_speed - m_actions[action_id2]->m_speed), 2) + 
            pow((m_actions[action_id1]->m_duration - m_actions[action_id2]->m_duration), 2) + 
            pow((m_actions[action_id1]->m_direction.x() - m_actions[action_id2]->m_direction.x()), 2) +
            pow((m_actions[action_id1]->m_direction.y() - m_actions[action_id2]->m_direction.y()), 2));
    } catch (const std::out_of_range& e) {
        std::cout<<"Out of Range error: "<<e.what();
    }
}

GenericAction* GenericActionSpace::get_action(const int& action_id) const {
    try {
        return m_actions[action_id];
    } catch (const std::out_of_range& e) {
        std::cout<<"Out of Range error: "<<e.what();
    }
}
}
}