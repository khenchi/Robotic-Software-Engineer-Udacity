#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
//TODO: Include the ball_chaser "DriveToTarget" header file
#include "ball_chaser/DriveToTarget.h"
// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities

bool drive_request_callback(ball_chaser::DriveToTarget::Request& req,
                            ball_chaser::DriveToTarget::Response& res)
{
  ROS_INFO("DriveToTarget Request received: linear_x: %1.2f, angular_z: %1.2f", (float)req.linear_x, (float)req.angular_z);

  // Publish command velocity to "/cmd_vel"
  geometry_msgs::Twist cmd_vel_;

  cmd_vel_.linear.x = req.linear_x; //  float64
  cmd_vel_.angular.z = req.angular_z; // float64
  
  motor_cmd_pub.publish(cmd_vel_);

  // Return a feedback message
  res.msg_feedback = "cmd_vel sent: linear_x = " + std::to_string(cmd_vel_.linear.x) + ", angular_z = " + std::to_string(cmd_vel_.angular.z);

  ROS_INFO_STREAM(res.msg_feedback);
  

  return true;

}



int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
  ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", drive_request_callback);
  ROS_INFO("Ready to send velocity values");

    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values
 //   while (ros::ok()) {
        // Create a motor_command object of type geometry_msgs::Twist
  //      geometry_msgs::Twist motor_command;
        // Set wheel velocities, forward [0.5, 0.0]
  //      motor_command.linear.x = 0.5;
  //      motor_command.angular.z = 0.0;
        // Publish angles to drive the robot
   //     motor_command_publisher.publish(motor_command);
    }

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}
