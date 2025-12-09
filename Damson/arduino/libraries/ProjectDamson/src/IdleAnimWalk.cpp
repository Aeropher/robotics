/*
 * File       Walk Idle Animations
 * Brief      Default gait (tripod) and Wave gait walk animations
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonIdle.h"
#include <Arduino.h>

// =================================================================================================
// DEFAULT GAIT (Tripod) - Uses built-in crawl functions
// The robot's default walking pattern moves 3 legs at once (tripod gait)
// =================================================================================================

void IdleAnimations::DefaultForwardBack()
{
  if (robotAction == nullptr) return;

  // 2 steps forward
  robotAction->CrawlForward();
  robotAction->CrawlForward();

  delay(300);

  // 2 steps back
  robotAction->CrawlBackward();
  robotAction->CrawlBackward();

  robotAction->InitialState();
}

void IdleAnimations::DefaultBackForward()
{
  if (robotAction == nullptr) return;

  // 2 steps back
  robotAction->CrawlBackward();
  robotAction->CrawlBackward();

  delay(300);

  // 2 steps forward
  robotAction->CrawlForward();
  robotAction->CrawlForward();

  robotAction->InitialState();
}

void IdleAnimations::DefaultLeftRight()
{
  if (robotAction == nullptr) return;

  // 2 steps left
  robotAction->CrawlLeft();
  robotAction->CrawlLeft();

  delay(300);

  // 2 steps right
  robotAction->CrawlRight();
  robotAction->CrawlRight();

  robotAction->InitialState();
}

void IdleAnimations::DefaultRightLeft()
{
  if (robotAction == nullptr) return;

  // 2 steps right
  robotAction->CrawlRight();
  robotAction->CrawlRight();

  delay(300);

  // 2 steps left
  robotAction->CrawlLeft();
  robotAction->CrawlLeft();

  robotAction->InitialState();
}

void IdleAnimations::DefaultTurnLeftRight()
{
  if (robotAction == nullptr) return;

  // Turn left
  robotAction->TurnLeft();
  robotAction->TurnLeft();

  delay(300);

  // Turn right
  robotAction->TurnRight();
  robotAction->TurnRight();

  robotAction->InitialState();
}

void IdleAnimations::DefaultTurnRightLeft()
{
  if (robotAction == nullptr) return;

  // Turn right
  robotAction->TurnRight();
  robotAction->TurnRight();

  delay(300);

  // Turn left
  robotAction->TurnLeft();
  robotAction->TurnLeft();

  robotAction->InitialState();
}

// =================================================================================================
// WAVE GAIT - Moves one leg at a time in sequence
// Slower but more stable, like a caterpillar wave
// Leg order: 1 -> 2 -> 3 -> 6 -> 5 -> 4 (alternating sides front to back)
// =================================================================================================

// Helper: Move a single leg forward/back in wave pattern
static void WaveStepLeg(RobotAction* robotAction, int leg, float yDir, float stepHeight, float stepLength)
{
  robotAction->LegMoveToRelatively(leg, Point(0, 0, stepHeight));        // Lift
  robotAction->LegMoveToRelatively(leg, Point(0, yDir * stepLength, 0)); // Move
  robotAction->LegMoveToRelatively(leg, Point(0, 0, -stepHeight));       // Lower
}

// Helper: Move a single leg left/right in wave pattern
static void WaveStepLegSide(RobotAction* robotAction, int leg, float xDir, float stepHeight, float stepLength)
{
  robotAction->LegMoveToRelatively(leg, Point(0, 0, stepHeight));        // Lift
  robotAction->LegMoveToRelatively(leg, Point(xDir * stepLength, 0, 0)); // Move
  robotAction->LegMoveToRelatively(leg, Point(0, 0, -stepHeight));       // Lower
}

void IdleAnimations::WaveForwardBack()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float stepLength = 25;

  // Wave forward: each leg steps forward in sequence
  WaveStepLeg(robotAction, 1, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 2, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 3, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 6, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 5, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 4, 1, stepHeight, stepLength);

  delay(300);

  // Wave backward: each leg steps backward in sequence
  WaveStepLeg(robotAction, 1, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 2, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 3, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 6, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 5, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 4, -1, stepHeight, stepLength);

  robotAction->InitialState();
}

void IdleAnimations::WaveBackForward()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float stepLength = 25;

  // Wave backward first
  WaveStepLeg(robotAction, 1, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 2, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 3, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 6, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 5, -1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 4, -1, stepHeight, stepLength);

  delay(300);

  // Then wave forward
  WaveStepLeg(robotAction, 1, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 2, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 3, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 6, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 5, 1, stepHeight, stepLength);
  WaveStepLeg(robotAction, 4, 1, stepHeight, stepLength);

  robotAction->InitialState();
}

void IdleAnimations::WaveLeftRight()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float stepLength = 20;

  // Wave left
  WaveStepLegSide(robotAction, 1, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 2, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 3, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 6, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 5, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 4, -1, stepHeight, stepLength);

  delay(300);

  // Wave right
  WaveStepLegSide(robotAction, 1, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 2, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 3, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 6, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 5, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 4, 1, stepHeight, stepLength);

  robotAction->InitialState();
}

void IdleAnimations::WaveRightLeft()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float stepLength = 20;

  // Wave right first
  WaveStepLegSide(robotAction, 1, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 2, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 3, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 6, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 5, 1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 4, 1, stepHeight, stepLength);

  delay(300);

  // Then wave left
  WaveStepLegSide(robotAction, 1, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 2, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 3, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 6, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 5, -1, stepHeight, stepLength);
  WaveStepLegSide(robotAction, 4, -1, stepHeight, stepLength);

  robotAction->InitialState();
}

void IdleAnimations::WaveTurnLeftRight()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float turnAmount = 15;

  // Turn left: right legs forward, left legs backward
  // Leg 1 (front-right) forward
  WaveStepLeg(robotAction, 1, 1, stepHeight, turnAmount);
  // Leg 4 (front-left) backward
  WaveStepLeg(robotAction, 4, -1, stepHeight, turnAmount);
  // Leg 2 (mid-right) forward
  WaveStepLeg(robotAction, 2, 1, stepHeight, turnAmount);
  // Leg 5 (mid-left) backward
  WaveStepLeg(robotAction, 5, -1, stepHeight, turnAmount);
  // Leg 3 (back-right) forward
  WaveStepLeg(robotAction, 3, 1, stepHeight, turnAmount);
  // Leg 6 (back-left) backward
  WaveStepLeg(robotAction, 6, -1, stepHeight, turnAmount);

  delay(300);

  // Turn right: left legs forward, right legs backward
  WaveStepLeg(robotAction, 4, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 1, -1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 5, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 2, -1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 6, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 3, -1, stepHeight, turnAmount);

  robotAction->InitialState();
}

void IdleAnimations::WaveTurnRightLeft()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float turnAmount = 15;

  // Turn right first: left legs forward, right legs backward
  WaveStepLeg(robotAction, 4, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 1, -1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 5, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 2, -1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 6, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 3, -1, stepHeight, turnAmount);

  delay(300);

  // Then turn left: right legs forward, left legs backward
  WaveStepLeg(robotAction, 1, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 4, -1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 2, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 5, -1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 3, 1, stepHeight, turnAmount);
  WaveStepLeg(robotAction, 6, -1, stepHeight, turnAmount);

  robotAction->InitialState();
}

// =================================================================================================
// LEGACY ALIASES - Keep old function names working
// =================================================================================================

void IdleAnimations::WalkForwardBack() { DefaultForwardBack(); }
void IdleAnimations::WalkLeftRight() { DefaultLeftRight(); }
void IdleAnimations::WalkTurnLeftRight() { DefaultTurnLeftRight(); }
void IdleAnimations::TripodForwardBack() { DefaultForwardBack(); }

#endif
