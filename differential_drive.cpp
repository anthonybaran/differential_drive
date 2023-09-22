/* This is written assuming everything is in polar coordinates. (r,theta)
Theta is used to used to give the robot a direction.
radius(r) is used to give the robot a speed or percent output.*/
#include <iostream>


double robot_width = 0.5;
double joy_radius;
double joy_angle;

// uses theta to create a turning circle for the robot with a max value of 8.5 meters
double output_turning_radius(double theta) 
{
    double outputMin = 0; // Minimum value of the output variable
    double outputMax = 8.5; // Maximum value of the output variable

    // Scale the input to the output range
    double scaleFactor = static_cast<double>(outputMax - outputMin) / 1.57;
    
    double output;
    if (theta > 0 && theta < 1.57) {
        output = static_cast<double>(scaleFactor * theta);
    }
    else if (theta > 1.57 && theta < 3.14) {
        output = static_cast<double>(scaleFactor * (theta - 2*(theta - 1.57)));
    }
    else if (theta > 3.14 && theta < 4.71) {
        output = static_cast<double>(scaleFactor * (theta - 3.14));
    }
    else if (theta > 4.71 && theta < 6.28) {
        output = static_cast<double>(scaleFactor * ((theta - 3.14) - 2*((theta - 3.14) - 1.57)));
    }
    else {
        output = 0;
    }

    return output;
}

// uses radius(r) to output a desired velocity with a max value of 50
double output_velocity(double joystick_radius)
{
    double outputMin = 0;
    double outputMax = 50;

    double scaleFactor = static_cast<double>(outputMax - outputMin) / 5.0; // divide by the max joystick radius
    double output = static_cast<double>(scaleFactor * joystick_radius);

    return output;
}

/* Calculates what the left and right wheel velocities should be based on the turning radius
and desired velocity of robot. Different cases for different quadrants of (r,theta). */
void calculate_wheel_velocities() 
{
    double right_Wheel_velocity;
    double left_Wheel_velocity;
    // turning straight right 
    if (joy_angle >= 0 && joy_angle <= 0.06) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) - (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) + (robot_width / 2));
    }
    // turning right
    else if (1.57 > joy_angle && joy_angle > 0.06) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
    }
    // going straight 
    else if (joy_angle == 1.57) {
        right_Wheel_velocity = output_velocity(joy_radius);
        left_Wheel_velocity = output_velocity(joy_radius);
    }
    // turning left 
    else if (joy_angle > 1.57 && joy_angle < 3.08) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
    }
    // turning straight left 
    else if (joy_angle >= 3.08 && joy_angle <= 3.20) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(3.10)) * (output_turning_radius(3.10) + (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(3.10)) * (output_turning_radius(3.10) - (robot_width / 2));
    }
    // turning left while going backwards
    else if (joy_angle > 3.20 && joy_angle < 4.71) {
        right_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
        left_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
    }
    // going straight backwards
    else if (joy_angle == 4.71) {
        right_Wheel_velocity = -output_velocity(joy_radius);
        left_Wheel_velocity = -output_velocity(joy_radius);
    }
    // turning right while going backwards
    else if (joy_angle > 4.71 && joy_angle < 6.22) {
        right_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
        left_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
    }
    // turing right
    else if (joy_angle >= 6.22) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) - (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) + (robot_width / 2));
    }

}