/*
 * File       Expressive Idle Animations
 * Brief      Stretch, ShakeOff, Yawn, TapFoot
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonIdle.h"
#include <Arduino.h>

// Stretch: Cat-like stretch - lower front, extend legs forward
void IdleAnimations::Stretch()
{
  if (robotAction == nullptr) return;

  const float stretchAngle = 10;  // degrees
  const float legExtend = 20;     // mm

  // Lower front (stretch pose)
  robotAction->TwistBody(Point(0, 0, 0), Point(stretchAngle, 0, 0));
  delay(200);

  // Extend front legs forward slightly
  robotAction->LegMoveToRelatively(1, Point(0, legExtend, 0));  // Front right
  robotAction->LegMoveToRelatively(4, Point(0, legExtend, 0));  // Front left
  delay(400);

  // Retract legs back before resetting
  robotAction->LegMoveToRelatively(1, Point(0, -legExtend, 0));
  robotAction->LegMoveToRelatively(4, Point(0, -legExtend, 0));

  robotAction->InitialState();
}

// ShakeOff: Quick side-to-side wiggle like shaking off water
void IdleAnimations::ShakeOff()
{
  if (robotAction == nullptr) return;

  const float shakeAngle = 10;  // degrees
  const int shakeCount = 4;

  for (int i = 0; i < shakeCount; i++) {
    // Shake left
    robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, shakeAngle));
    delay(80);

    // Shake right
    robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, -shakeAngle * 2));
    delay(80);

    // Back to center
    robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, shakeAngle));
  }

  robotAction->InitialState();
}

// Yawn: Bow motion - lower front, hold, return
void IdleAnimations::Yawn()
{
  if (robotAction == nullptr) return;

  const float bowAngle = 12;  // degrees

  // Lower front of body (bow down)
  robotAction->TwistBody(Point(0, 0, 0), Point(bowAngle, 0, 0));
  delay(600);  // Hold the yawn

  robotAction->InitialState();
}

// TapFoot: Impatient foot tapping
void IdleAnimations::TapFoot(int leg)
{
  if (robotAction == nullptr) return;

  const float tapHeight = 15;  // mm
  const int tapCount = 4;

  for (int i = 0; i < tapCount; i++) {
    // Raise leg
    robotAction->LegMoveToRelatively(leg, Point(0, 0, tapHeight));

    // Tap down
    robotAction->LegMoveToRelatively(leg, Point(0, 0, -tapHeight));

    // Brief pause between taps
    if (i < tapCount - 1) {
      delay(100);
    }
  }

  robotAction->InitialState();
}

#endif
