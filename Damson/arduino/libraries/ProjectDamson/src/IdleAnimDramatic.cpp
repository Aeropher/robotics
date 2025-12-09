/*
 * File       Alert/Dramatic Idle Animations
 * Brief      Startle, PounceReady, VictoryPose
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonIdle.h"
#include <Arduino.h>

// Startle: Quick crouch, freeze, then rise
void IdleAnimations::Startle()
{
  if (robotAction == nullptr) return;

  const float crouchHeight = 20;  // mm

  // Quick crouch down
  robotAction->TwistBody(Point(0, 0, -crouchHeight), Point(0, 0, 0));

  // Freeze (hold the startled pose)
  delay(500);

  robotAction->InitialState();
}

// PounceReady: Stalking pose - lower front, raise rear
void IdleAnimations::PounceReady()
{
  if (robotAction == nullptr) return;

  const float pounceAngle = 15;  // degrees

  // Lower front, raise rear (tilt forward)
  robotAction->TwistBody(Point(0, 0, 0), Point(pounceAngle, 0, 0));

  // Hold dramatic pose
  delay(800);

  robotAction->InitialState();
}

// VictoryPose: Rise up tall and proud
void IdleAnimations::VictoryPose()
{
  if (robotAction == nullptr) return;

  const float riseHeight = 25;  // mm
  const float tiltBack = 8;     // degrees - lean back triumphantly

  // Rise up and lean back slightly
  robotAction->TwistBody(Point(0, 0, riseHeight), Point(-tiltBack, 0, 0));

  // Hold triumphant pose
  delay(1000);

  robotAction->InitialState();
}

// DrumFingers: Tap each foot in sequence like drumming fingers on a table
void IdleAnimations::DrumFingers()
{
  if (robotAction == nullptr) return;

  const float stompHeight = 20;  // mm
  const int rounds = 2;
  // Leg order: 1 (front-right), 2 (mid-right), 3 (back-right),
  //            6 (back-left), 5 (mid-left), 4 (front-left)
  const int legOrder[] = {1, 2, 3, 6, 5, 4};
  const int legCount = 6;

  for (int round = 0; round < rounds; round++) {
    for (int i = 0; i < legCount; i++) {
      int leg = legOrder[i];

      // Raise leg
      robotAction->LegMoveToRelatively(leg, Point(0, 0, stompHeight));

      // Stomp down
      robotAction->LegMoveToRelatively(leg, Point(0, 0, -stompHeight));

      // Brief pause between stomps
      delay(80);
    }
  }

  robotAction->InitialState();
}

// StandTall: Rise up as high as possible
void IdleAnimations::StandTall()
{
  if (robotAction == nullptr) return;

  const float maxHeight = 35;  // mm - maximum safe height

  // Rise up to full height
  robotAction->TwistBody(Point(0, 0, maxHeight), Point(0, 0, 0));

  // Hold tall pose
  delay(1200);

  robotAction->InitialState();
}

// LieDown: Lower body and stretch all legs outward
void IdleAnimations::LieDown()
{
  if (robotAction == nullptr) return;

  const float lowerHeight = -25;  // mm - lower body
  const float legExtend = 15;     // mm - extend legs outward

  // Lower body down
  robotAction->TwistBody(Point(0, 0, lowerHeight), Point(0, 0, 0));
  delay(200);

  // Stretch all legs outward (away from body)
  robotAction->LegMoveToRelatively(1, Point(-legExtend, legExtend, 0));   // Front right - out and forward
  robotAction->LegMoveToRelatively(2, Point(-legExtend, 0, 0));           // Mid right - out
  robotAction->LegMoveToRelatively(3, Point(-legExtend, -legExtend, 0));  // Back right - out and back
  robotAction->LegMoveToRelatively(4, Point(legExtend, legExtend, 0));    // Front left - out and forward
  robotAction->LegMoveToRelatively(5, Point(legExtend, 0, 0));            // Mid left - out
  robotAction->LegMoveToRelatively(6, Point(legExtend, -legExtend, 0));   // Back left - out and back

  // Hold relaxed pose
  delay(1500);

  // Retract all legs back before resetting
  robotAction->LegMoveToRelatively(1, Point(legExtend, -legExtend, 0));
  robotAction->LegMoveToRelatively(2, Point(legExtend, 0, 0));
  robotAction->LegMoveToRelatively(3, Point(legExtend, legExtend, 0));
  robotAction->LegMoveToRelatively(4, Point(-legExtend, -legExtend, 0));
  robotAction->LegMoveToRelatively(5, Point(-legExtend, 0, 0));
  robotAction->LegMoveToRelatively(6, Point(-legExtend, legExtend, 0));

  robotAction->InitialState();
}

#endif
