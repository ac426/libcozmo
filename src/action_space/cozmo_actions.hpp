#ifndef COZMO_ACTIONS_HPP
#define COZMO_ACTIONS_HPP

#include <vector>

struct Action
{
    double lin_vel;
    double ang_vel;
    double duration;
};

struct Pose
{
    double x;
    double y;
    double z;
    double angle_z;
};

struct Point
{
    double x;
    double y;
};

struct Object_Oriented_Action
{
    Pose pose;
    Action action;
};


// Linear interpolation following MATLAB linspace
std::vector<double> generate_samples(double min, double max, std::size_t N);

class GenericActionSpace {
    public:
        GenericActionSpace(double lin_min,
                           double lin_max,
                           double lin_samples,
                           double ang_min,
                           double ang_max,
                           double ang_samples,
                           double dur_min,
                           double dur_max,
                           double dur_samples);

        Action get_action(int action_id);

        std::vector<Action> get_action_space();

        void view_action_space();
    private:
        double lin_min;
        double lin_max;
        double lin_samples;
        double ang_min;
        double ang_max;
        double ang_samples;
        double dur_min;
        double dur_max;
        double dur_samples;
        std::vector<Action> actions;

        void generate_actions();
};

class ObjectOrientedActionSpace {
    public:
        ObjectOrientedActionSpace(Pose pose,
                                  int samples,
                                  double lin_min,
                                  double lin_max,
                                  double lin_samples,
                                  double dur_min,
                                  double dur_max,
                                  double dur_samples);
        
        Object_Oriented_Action get_action(int action_id);

        std::vector<Object_Oriented_Action> get_action_space();
        
        void view_action_space();

    private:
        Pose pose;
        double samples;
        double lin_min;
        double lin_max;
        double lin_samples;
        double dur_min;
        double dur_max;
        double dur_samples;
        std::vector<Object_Oriented_Action> actions;

        Point cube_offset(double offset, double angle);
        
        /*
        Helper function to find the location of all 4 sides of the cube

        Parameters
        ----------
        angle : the angle of the cube, in radians
        
        returns a sorted list of the angle of each of the 4 sides where index
            0 corresponds to front of cube
            1 corresponds to left of cube
            2 corresponds to back of cube
            3 corresponds to right of cube
        */
        std::vector<double> find_sides(double angle);

        void generate_actions(double h_offset=40, double v_offset=60);

        /*
        Helper function to generate cube offset positions

        Parameters
        ----------
        h_offset : the max horizontal offset from the center of the edge of the cube, in millimeters
        v_offset : the vertical offset away from the center of the cube, in millimeters
        */
        std::vector<Pose> generate_offsets(double h_offset, double v_offset);

        /** Helper function to find the value closest to zero in a list,
            used in find_sides to identify which angle of the cube
            is the front

            Note: in case the corner of the cube is perfectly in align with cozmo
            and there is no closest side, we choose the right side to be the front

            returns the index of the value closest to zero
        */
        int nearest_zero(std::vector<double> values);
};

#endif
