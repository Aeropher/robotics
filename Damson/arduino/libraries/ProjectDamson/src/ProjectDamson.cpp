/*
 * File       Main class for Project Damson Hexapod Robot
 * Based on   Freenove Hexapod Robot library by Ethan Pan @ Freenove
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 *            (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamson.h"

ProjectDamson::ProjectDamson() {}

void ProjectDamson::Start(bool commFunction)
{
  communication.Start(commFunction);
  idle.SetRobotAction(&communication.robotAction);
}

void ProjectDamson::Update()
{
  if (communication.commFunction)
    communication.UpdateOrder();
}

void ProjectDamson::SetWiFi(String name, String password)
{
  communication.SetWiFi(name, password);
}

void ProjectDamson::SetWiFiChannel(byte channel)
{
  communication.SetWiFiChannel(channel);
}

void ProjectDamson::ActiveMode()
{
  if (!communication.commFunction)
    communication.robotAction.ActiveMode();
}

void ProjectDamson::SleepMode()
{
  if (!communication.commFunction)
    communication.robotAction.SleepMode();
}

void ProjectDamson::SwitchMode()
{
  if (!communication.commFunction)
    communication.robotAction.SwitchMode();
}

void ProjectDamson::CrawlForward()
{
  if (!communication.commFunction)
    communication.robotAction.CrawlForward();
}

void ProjectDamson::CrawlBackward()
{
  if (!communication.commFunction)
    communication.robotAction.CrawlBackward();
}

void ProjectDamson::CrawlLeft()
{
  if (!communication.commFunction)
    communication.robotAction.CrawlLeft();
}

void ProjectDamson::CrawlRight()
{
  if (!communication.commFunction)
    communication.robotAction.CrawlRight();
}

void ProjectDamson::TurnLeft()
{
  if (!communication.commFunction)
    communication.robotAction.TurnLeft();
}

void ProjectDamson::TurnRight()
{
  if (!communication.commFunction)
    communication.robotAction.TurnRight();
}

void ProjectDamson::Crawl(float x, float y, float angle)
{
  if (!communication.commFunction)
    communication.robotAction.Crawl(x, y, angle);
}

void ProjectDamson::ChangeBodyHeight(float height)
{
  if (!communication.commFunction)
    communication.robotAction.ChangeBodyHeight(height);
}

void ProjectDamson::MoveBody(float x, float y, float z)
{
  if (!communication.commFunction)
    communication.robotAction.MoveBody(x, y, z);
}

void ProjectDamson::RotateBody(float x, float y, float z)
{
  if (!communication.commFunction)
    communication.robotAction.RotateBody(x, y, z);
}

void ProjectDamson::TwistBody(float xMove, float yMove, float zMove, float xRotate, float yRotate, float zRotate)
{
  if (!communication.commFunction)
    communication.robotAction.TwistBody(Point(xMove, yMove, zMove), Point(xRotate, yRotate, zRotate));
}

void ProjectDamson::LegMoveToRelatively(int leg, float x, float y, float z)
{
  if (!communication.commFunction)
    communication.robotAction.LegMoveToRelatively(leg, Point(x, y, z));
}

void ProjectDamson::SetActionSpeed(float speed)
{
  if (!communication.commFunction)
    communication.robotAction.SetSpeedMultiple(speed / 100);
}

void ProjectDamson::SetActionGroup(int group)
{
  if (!communication.commFunction)
    communication.robotAction.SetActionGroup(group);
}

void ProjectDamson::StartupShake(int leg, int count)
{
  // Directly access robotAction to bypass commFunction check
  for (int i = 0; i < count; i++) {
    communication.robotAction.LegMoveToRelatively(leg, Point(0, 0, 30));  // Raise leg
    communication.robotAction.LegMoveToRelatively(leg, Point(0, 0, -30)); // Lower leg
  }
}

void ProjectDamson::Jump()
{
  // Jump attempt: crouch down quickly, then explosively extend all legs
  // The idea is to build potential energy in crouch, then release it fast

  const float crouchDepth = -40;   // mm - crouch down
  const float jumpHeight = 60;     // mm - extend legs for jump

  // Phase 1: Quick crouch to build potential energy
  communication.robotAction.TwistBody(Point(0, 0, crouchDepth), Point(0, 0, 0));

  // Phase 2: Explosive extension - push off!
  // Move as fast as possible by using large movement
  communication.robotAction.TwistBody(Point(0, 0, jumpHeight + (-crouchDepth)), Point(0, 0, 0));

  // Phase 3: Return to normal after landing
  delay(300);  // Air time + landing

  communication.robotAction.InitialState();
}

#endif
