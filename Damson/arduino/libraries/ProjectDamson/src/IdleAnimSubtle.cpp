/*
 * File       Subtle/Ambient Idle Animations
 * Brief      Breathing, WeightShift, LookAround
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonIdle.h"
#include <Arduino.h>

// Breathing: Gentle body rise and fall
void IdleAnimations::Breathing()
{
  if (robotAction == nullptr) return;

  const float breathHeight = 20;  // mm total rise

  // Inhale - rise up
  robotAction->TwistBody(Point(0, 0, breathHeight), Point(0, 0, 0));

  // Hold at top of breath
  delay(800);

  // Exhale - lower back down
  robotAction->TwistBody(Point(0, 0, -breathHeight), Point(0, 0, 0));

  // Hold at bottom
  delay(600);

  robotAction->InitialState();
}

// WeightShift: Slow side-to-side body sway
void IdleAnimations::WeightShift()
{
  if (robotAction == nullptr) return;

  const float shiftAmount = 15;  // mm

  // Shift left
  robotAction->TwistBody(Point(-shiftAmount, 0, 0), Point(0, 0, 0));
  delay(400);

  // Shift right
  robotAction->TwistBody(Point(shiftAmount * 2, 0, 0), Point(0, 0, 0));
  delay(400);

  robotAction->InitialState();
}

// LookAround: Tilt body as if scanning the environment
void IdleAnimations::LookAround()
{
  if (robotAction == nullptr) return;

  const float tiltAngle = 8;  // degrees

  // Look left
  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, tiltAngle));
  delay(300);

  // Look right
  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, -tiltAngle * 2));
  delay(300);

  // Look up (tilt back)
  robotAction->TwistBody(Point(0, 0, 0), Point(-tiltAngle, 0, tiltAngle));
  delay(400);

  robotAction->InitialState();
}

#endif
