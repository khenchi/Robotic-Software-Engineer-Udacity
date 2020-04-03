#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
  // TODO: Request a service and pass the velocities to it to drive the robot
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;

  // Call the drive_bot service and pass the requested velocities
  if (!srvClient.call(srv)) 
  {
    ROS_ERROR("Failed to call service drive_bot!");
  }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    float lin_x = 0;
    float ang_z = 0;
    bool is_ball_found = false;


    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera

  // First step: loop through the pixels to find the white pixel (= 255)
  
  // Initialization: assume the white ball is not detected

  for (int i = 0; i < img.step * img.height; i++)
  {
    if (img.data[i] == white_pixel) 
    {
	    is_ball_found = true;
	    int row_found = i % img.step;
	    if (row_found < img.step / 3) 
	    {
            // Turn Left
            lin_x = 0.0;
            ang_z = 0.2;
	    }
	    else if (row_found > img.step * 2 / 3) 
            {
            // Turn Right
            lin_x = 0.0;
            ang_z = -0.2;
	    }
	    else 
            {
            // Move Forward
            lin_x = 0.5;
    	    ang_z = 0.0;
	    }
	    break;
   }
  }
  
             
  // Step III: if white ball is found, call drive_bot function and pass velocities
  drive_robot(lin_x, ang_z); // drive the bot
  

  // Publish some info
  if (is_ball_found)
	{ 
    ROS_INFO_STREAM("Target detected: driving the bot to the target ...");
	}
 else 
	{
    ROS_INFO_STREAM("Target missing: Stop the bot for now.");
	}




int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
