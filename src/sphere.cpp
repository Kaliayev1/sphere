//===================================================
// Name        : sphere.cpp
// Author      : Antoine & Adrien
// Version     :
// Copyright   : 
// Description : prog principal en charge de la regulation de la sphere
//===================================================

#include "modele.h"

geometry_msgs::Vector3 gvCmd; //vecteur cmd utilisateur .x vitesse .y direction
geometry_msgs::Vector3 gvPosA;// vecteur qui contient les positions angulaire 
geometry_msgs::Vector3 gvCmdMoteurs; // vecteur qui contient les commandes envoyées à l'arduino

using namespace std;

void CmdVitesse(const geometry_msgs::Twist& lvVit)
{
      gvCmd.x = lvVit.linear.x;       
}


void CmdDirection(const geometry_msgs::Twist& lvDir)
{
      gvCmd.y = lvDir.linear.x;       
}


void Mpu(const geometry_msgs::Twist& lvMpu)
{
      gvPosA.x = lvMpu.linear.x;
      gvPosA.y = lvMpu.linear.y;            
}

void Btn(const std_msgs::Bool& lbBtn)
{
         system("mplayer /home/ros/catkin_ws/src/sphere/audio/sound.wav"); 
}

void KillBtn(const std_msgs::Bool& lbKill)
{
        system("rosnode kill -a");    
}

int main(int argc, char **argv){
  
  double KpVit, KdVit, KiVit, KpDir, KdDir, KiDir;
  
  ros::init(argc, argv, "filter_velocity");
  ros::NodeHandle nh;
  ros::Rate rate(F);
  
  ros::Subscriber sub_joy_1 = nh.subscribe("/cmd_vel1",1000,&CmdVitesse);
  ros::Subscriber sub_joy_2 = nh.subscribe("/cmd_vel2",1000,&CmdDirection);
  ros::Subscriber sub_mpu = nh.subscribe("/mpu_Apos",1000,&Mpu);
  ros::Subscriber sub_btn = nh.subscribe("/btn_press",1000,&Btn);
  ros::Subscriber sub_kill = nh.subscribe("/btn_kill",1000,&KillBtn);
  
  ros::Publisher pub_CmdMoteurs = nh.advertise<geometry_msgs::Vector3>("/cmd_moteur", 1000);
  ros::Publisher pub_gvCmd = nh.advertise<geometry_msgs::Vector3>("/debug", 1000);
  
  
  nh.param("KpVit", KpVit, 0.1);
  nh.param("KdVit", KdVit, 0.01);
  nh.param("KiVit", KiVit, 0.5);
  nh.param("KpDir", KpDir, 0.1);
  nh.param("KdDir", KdDir, 0.01);
  nh.param("KiDir", KiDir, 0.5);
  
  PID pid_vit = PID(1/F, JoyMax, JoyMin, KpVit, KdVit, KiVit);
  PID pid_dir = PID(1/F, JoyMax, JoyMin, KpDir, KdDir, KiDir);

  while(ros::ok()){
    
    gvCmdMoteurs.x = pid_vit.calculate(gvCmd.x, gvPosA.x);
    gvCmdMoteurs.y = pid_dir.calculate(gvCmd.y, gvPosA.y);
    
    pub_CmdMoteurs.publish(gvCmdMoteurs);
    pub_gvCmd.publish(gvCmd); //pour debug
      
    rate.sleep();
    ros::spinOnce();      
  } 
}
