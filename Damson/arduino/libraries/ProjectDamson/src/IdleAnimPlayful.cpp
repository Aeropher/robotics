/*
 * File       Playful Idle Animations
 * Brief      Wave, DanceWiggle, CuriousPeek, HappyBounce
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonIdle.h"
#include <Arduino.h>

// Wave: Raise a leg and wave it side to side
void IdleAnimations::Wave(int leg)
{
  if (robotAction == nullptr) return;

  const float liftHeight = 50;   // mm - lift high off ground
  const float waveAmount = 25;   // mm side to side
  const int waveCount = 3;

  // First lift leg straight up off the ground
  robotAction->LegMoveToRelatively(leg, Point(0, 0, liftHeight));
  delay(200);  // Pause to show leg is raised

  // Wave side to side
  for (int i = 0; i < waveCount; i++) {
    robotAction->LegMoveToRelatively(leg, Point(waveAmount, 0, 0));
    delay(150);
    robotAction->LegMoveToRelatively(leg, Point(-waveAmount * 2, 0, 0));
    delay(150);
    robotAction->LegMoveToRelatively(leg, Point(waveAmount, 0, 0));
  }

  // Lower leg back down before resetting
  robotAction->LegMoveToRelatively(leg, Point(0, 0, -liftHeight));

  robotAction->InitialState();
}

// DanceWiggle: Rhythmic sway with bounces
void IdleAnimations::DanceWiggle()
{
  if (robotAction == nullptr) return;

  const float swayAmount = 10;    // mm
  const float bounceHeight = 8;   // mm
  const int beatCount = 4;

  for (int i = 0; i < beatCount; i++) {
    // Sway left and up
    robotAction->TwistBody(Point(-swayAmount, 0, bounceHeight), Point(0, 0, 5));
    delay(150);

    // Sway right and down
    robotAction->TwistBody(Point(swayAmount * 2, 0, -bounceHeight * 2), Point(0, 0, -10));
    delay(150);

    // Back to center and up
    robotAction->TwistBody(Point(-swayAmount, 0, bounceHeight), Point(0, 0, 5));
  }

  robotAction->InitialState();
}

// CuriousPeek: Lean forward and look around curiously
void IdleAnimations::CuriousPeek()
{
  if (robotAction == nullptr) return;

  const float leanForward = 15;  // mm
  const float tiltAngle = 10;    // degrees

  // Lean forward
  robotAction->TwistBody(Point(0, leanForward, 0), Point(0, 0, 0));
  delay(200);

  // Tilt left (curious look)
  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, tiltAngle));
  delay(300);

  // Tilt right
  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, -tiltAngle * 2));
  delay(300);

  robotAction->InitialState();
}

// HappyBounce: Quick energetic bouncing
void IdleAnimations::HappyBounce(int count)
{
  if (robotAction == nullptr) return;

  const float bounceHeight = 15;  // mm

  for (int i = 0; i < count; i++) {
    // Bounce up
    robotAction->TwistBody(Point(0, 0, bounceHeight), Point(0, 0, 0));

    // Bounce down
    robotAction->TwistBody(Point(0, 0, -bounceHeight), Point(0, 0, 0));

    // Brief pause between bounces
    if (i < count - 1) {
      delay(50);
    }
  }

  robotAction->InitialState();
}

#endif
