/*
Dzikri Purnama
Elektronika dan Instrumentasi - Universitas Gadjah Mada

Keyboard dengan getche.
*/

#include <ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "keyboard.h"
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <termios.h>
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>

char k=' ';
using namespace std;
using namespace cv;

    geometry_msgs::Twist twist_msg;
    geometry_msgs::Twist hover;
    geometry_msgs::Twist maju;
    geometry_msgs::Twist mundur;
    geometry_msgs::Twist rotasika;
    geometry_msgs::Twist rotasiki;
    geometry_msgs::Twist geserka;
    geometry_msgs::Twist geserki;
    geometry_msgs::Twist naik;
    geometry_msgs::Twist turun;
    std_msgs::Empty msg;

void chatterCallback( const sensor_msgs::ImageConstPtr & newimage)
{
    Mat frame;

    cv_bridge::CvImagePtr bridge;
    bridge = cv_bridge::toCvCopy(newimage, sensor_msgs::image_encodings::BGR8);
    frame = bridge->image;

    imshow("camera",frame);

    int pressedkey;
    pressedkey = waitKey(1);

}

void KeyboardPressedCallback(keyboard::Key key)
{
    switch(key.code)
    {
        case keyboard::Key::KEY_a:
            k='a';
            break;
        case keyboard::Key::KEY_s:
            k='s';
            break;
        case keyboard::Key::KEY_d:
            k='d';
            break;
        case keyboard::Key::KEY_w:
            k='w';
            break;
        case keyboard::Key::KEY_q:
            k='q';
            break;
        case keyboard::Key::KEY_e:
            k='e';
            break;
        case keyboard::Key::KEY_i:
            k='i';
            break;
        case keyboard::Key::KEY_k:
            k='k';
            break;
        case keyboard::Key::KEY_j:
            k='j';
            break;
        case keyboard::Key::KEY_l:
            k='l';
            break;
        case keyboard::Key::KEY_h:
            k='h';
            break;
        case keyboard::Key::KEY_r:
            k='r';
            break;
        default:
            k='h';
            break;
    }
}

void KeyboardReleasedCallback(keyboard::Key key)
{
    switch (key.code) {
    case keyboard::Key::KEY_l:
        exit(0);
        break;
    case keyboard::Key::KEY_r:
        exit(0);
        break;
    default:
        k='h';
        break;
    }
}

int main(int argc, char** argv)
{

    printf("Manual Test Node Starting");
    ros::init(argc, argv,"ARDrone_manual_test");
    ros::NodeHandle node;
    ros::NodeHandle keyboarddown;
    ros::NodeHandle keyboardup;
    ros::NodeHandle camera;
    ros::Rate loop_rate(50);
    image_transport::ImageTransport it(camera);
    image_transport::Subscriber it_sub = it.subscribe("ardrone/image_raw", 1, chatterCallback);
    ros::Subscriber kbddwn = keyboarddown.subscribe("/keyboard/keydown",1,KeyboardPressedCallback);
    ros::Subscriber kbdup = keyboardup.subscribe("/keyboard/keyup",1,KeyboardReleasedCallback);
    ros::Publisher pub_empty_takeoff;
    ros::Publisher pub_empty_land;
    ros::Publisher pub_empty_reset;
    ros::Publisher pub_twist;
/*
    -linear.x: move backward
    +linear.x: move forward
    -linear.y: move right
    +linear.y: move left
    -linear.z: move down
    +linear.z: move up

    -angular.z: turn left
    +angular.z: turn right
*/

    //hover message
                hover.linear.x=0.0;
                hover.linear.y=0.0;
                hover.linear.z=0.0;
                hover.angular.x=0.0;
                hover.angular.y=0.0;
                hover.angular.z=0.0;

    //maju message
                maju.linear.x=0.1;
                maju.linear.y=0.0;
                maju.linear.z=0.0;
                maju.angular.x=0.0;
                maju.angular.y=0.0;
                maju.angular.z=0.0;

    //mundur message
                mundur.linear.x=-0.1;
                mundur.linear.y=0.0;
                mundur.linear.z=0.0;
                mundur.angular.x=0.0;
                mundur.angular.y=0.0;
                mundur.angular.z=0.0;

    //geser kanan message
                geserka.linear.x=0.0;
                geserka.linear.y=-0.1;
                geserka.linear.z=0.0;
                geserka.angular.x=0.0;
                geserka.angular.y=0.0;
                geserka.angular.z=0.0;

    //geser kiri message
                geserki.linear.x=0.0;
                geserki.linear.y=0.1;
                geserki.linear.z=0.0;
                geserki.angular.x=0.0;
                geserki.angular.y=0.0;
                geserki.angular.z=0.0;


    //rotasi kanan message
                rotasika.linear.x=0.0;
                rotasika.linear.y=0.0;
                rotasika.linear.z=0.0;
                rotasika.angular.x=0.0;
                rotasika.angular.y=0.0;
                rotasika.angular.z=-0.1;

    //rotasi kiri message
                rotasiki.linear.x=0.0;
                rotasiki.linear.y=0.0;
                rotasiki.linear.z=0.0;
                rotasiki.angular.x=0.0;
                rotasiki.angular.y=0.0;
                rotasiki.angular.z=+0.1;

    //naik message
                naik.linear.x=0.0;
                naik.linear.y=0.0;
                naik.linear.z=+0.1;
                naik.angular.x=0.0;
                naik.angular.y=0.0;
                naik.angular.z=0.0;

    //turun message
                turun.linear.x=0.0;
                turun.linear.y=0.0;
                turun.linear.z=-0.1;
                turun.angular.x=0.0;
                turun.angular.y=0.0;
                turun.angular.z=0.0;

        pub_twist         = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);       /* Message queue length is just 1 */
        pub_empty_takeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);    /* Message queue length is just 1 */
        pub_empty_land    = node.advertise<std_msgs::Empty>("/ardrone/land", 1);       /* Message queue length is just 1 */
        pub_empty_reset   = node.advertise<std_msgs::Empty>("/ardrone/reset", 1);      /* Message queue length is just 1 */


    ROS_INFO("Starting ARdrone_test loop");
    cout<<"Map keyboard:\n"
        <<"a = geser kiri        i = naik\n"
        <<"s = mundur            k = turun\n"
        <<"d = geser kanan       j = takeoff\n"
        <<"w = maju              l = landing\n"
        <<"q = rotasi kiri       h = hover\n"
        <<"e = rotasi kanan      r = reset\n";

while (ros::ok())
{
        switch(k)
        {
        case 'j':
            pub_empty_takeoff.publish(msg); //launches the drone
            pub_twist.publish(hover); //drone is flat
            ROS_INFO("Taking off");
            ros::spinOnce();
            loop_rate.sleep();
            k=' ';
            break;

        case 'l':
            pub_twist.publish(hover); //drone is flat
            pub_empty_land.publish(msg); //lands the drone
            ROS_INFO("Landing");
            exit(0);
            k = ' ';
            break;

        case 'h':
            ROS_INFO("hover");
            pub_twist.publish(hover);
            k = ' ';
            break;

        case 'w':
            ROS_INFO("gerak maju");
            pub_twist.publish(maju);
            k=' ';
            break;

        case 's':
            ROS_INFO("gerak mundur");
            pub_twist.publish(mundur);
            k = ' ';
            break;

        case 'd':
            ROS_INFO("gerak kanan");
            pub_twist.publish(geserka);
            k = ' ';
            break;

        case 'a':
            ROS_INFO("gerak kiri");
            pub_twist.publish(geserki);
            k = ' ';
            break;

        case 'e':
            ROS_INFO("rotasi ke kanan");
            pub_twist.publish(rotasika);
            k = ' ';
            break;

        case 'q':
            ROS_INFO("rotasi ke kiri");
            pub_twist.publish(rotasiki);
            k = ' ';
            break;

        case 'i':
            ROS_INFO("naik");
            pub_twist.publish(naik);
            k = ' ';
            break;

        case 'k':
            ROS_INFO("turun");
            pub_twist.publish(turun);
            k = ' ';
            break;
        }
    ros::spinOnce();
    loop_rate.sleep();

}//ros::ok

}//main
