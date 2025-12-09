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
// Each cycle: lift leg, move it forward, lower it, then push body forward with all legs
// Leg order for forward: back legs first (3,6), then middle (2,5), then front (1,4)
// This creates a wave that pushes the body forward
// =================================================================================================

// Wave gait step: reposition one leg, then push body
// For forward motion: leg moves forward (positive Y), then all legs push back (negative Y on body = forward motion)
static void WaveStep(RobotAction* robotAction, int leg, float stepHeight, float legReach, float bodyPush)
{
  // 1. Lift this leg
  robotAction->LegMoveToRelatively(leg, Point(0, 0, stepHeight));

  // 2. Move leg forward (to where it will plant)
  robotAction->LegMoveToRelatively(leg, Point(0, legReach, 0));

  // 3. Lower leg
  robotAction->LegMoveToRelatively(leg, Point(0, 0, -stepHeight));

  // 4. Push body forward by moving all legs backward relative to body
  //    Using TwistBody to shift body forward (which moves legs backward in body frame)
  robotAction->TwistBody(Point(0, bodyPush, 0), Point(0, 0, 0));
}

// Wave gait step for lateral movement
static void WaveStepSide(RobotAction* robotAction, int leg, float stepHeight, float legReach, float bodyPush)
{
  robotAction->LegMoveToRelatively(leg, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(leg, Point(legReach, 0, 0));
  robotAction->LegMoveToRelatively(leg, Point(0, 0, -stepHeight));
  robotAction->TwistBody(Point(bodyPush, 0, 0), Point(0, 0, 0));
}

void IdleAnimations::WaveForwardBack()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float legReach = 15;    // How far leg reaches forward
  const float bodyPush = 5;     // How much body moves per step (constrained by TwistBody limits)

  // Wave forward: back to front, one leg at a time
  // Each leg reaches forward, plants, then body shifts forward
  WaveStep(robotAction, 3, stepHeight, legReach, bodyPush);  // back-right
  WaveStep(robotAction, 6, stepHeight, legReach, bodyPush);  // back-left
  WaveStep(robotAction, 2, stepHeight, legReach, bodyPush);  // mid-right
  WaveStep(robotAction, 5, stepHeight, legReach, bodyPush);  // mid-left
  WaveStep(robotAction, 1, stepHeight, legReach, bodyPush);  // front-right
  WaveStep(robotAction, 4, stepHeight, legReach, bodyPush);  // front-left

  delay(300);

  // Wave backward: front to back
  WaveStep(robotAction, 1, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 4, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 2, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 5, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 3, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 6, stepHeight, -legReach, -bodyPush);

  robotAction->InitialState();
}

void IdleAnimations::WaveBackForward()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float legReach = 15;
  const float bodyPush = 5;

  // Wave backward first: front to back
  WaveStep(robotAction, 1, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 4, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 2, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 5, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 3, stepHeight, -legReach, -bodyPush);
  WaveStep(robotAction, 6, stepHeight, -legReach, -bodyPush);

  delay(300);

  // Then wave forward: back to front
  WaveStep(robotAction, 3, stepHeight, legReach, bodyPush);
  WaveStep(robotAction, 6, stepHeight, legReach, bodyPush);
  WaveStep(robotAction, 2, stepHeight, legReach, bodyPush);
  WaveStep(robotAction, 5, stepHeight, legReach, bodyPush);
  WaveStep(robotAction, 1, stepHeight, legReach, bodyPush);
  WaveStep(robotAction, 4, stepHeight, legReach, bodyPush);

  robotAction->InitialState();
}

void IdleAnimations::WaveLeftRight()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float legReach = 12;
  const float bodyPush = 4;

  // Wave left (negative X direction for body)
  WaveStepSide(robotAction, 1, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 4, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 2, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 5, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 3, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 6, stepHeight, -legReach, -bodyPush);

  delay(300);

  // Wave right
  WaveStepSide(robotAction, 1, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 4, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 2, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 5, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 3, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 6, stepHeight, legReach, bodyPush);

  robotAction->InitialState();
}

void IdleAnimations::WaveRightLeft()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float legReach = 12;
  const float bodyPush = 4;

  // Wave right first
  WaveStepSide(robotAction, 1, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 4, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 2, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 5, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 3, stepHeight, legReach, bodyPush);
  WaveStepSide(robotAction, 6, stepHeight, legReach, bodyPush);

  delay(300);

  // Then wave left
  WaveStepSide(robotAction, 1, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 4, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 2, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 5, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 3, stepHeight, -legReach, -bodyPush);
  WaveStepSide(robotAction, 6, stepHeight, -legReach, -bodyPush);

  robotAction->InitialState();
}

void IdleAnimations::WaveTurnLeftRight()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float legReach = 10;

  // Turn left: right side legs forward, left side legs backward
  // Reposition legs first
  robotAction->LegMoveToRelatively(1, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(1, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(1, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(4, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(4, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(2, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(2, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(2, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(5, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(5, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(3, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(3, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(6, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(6, Point(0, 0, -stepHeight));

  // Rotate body to complete the turn
  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, -10));

  delay(300);

  // Turn right: opposite
  robotAction->LegMoveToRelatively(1, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(1, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(1, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(4, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(4, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(2, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(2, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(2, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(5, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(5, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(3, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(3, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(6, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(6, Point(0, 0, -stepHeight));

  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, 10));

  robotAction->InitialState();
}

void IdleAnimations::WaveTurnRightLeft()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;
  const float legReach = 10;

  // Turn right first
  robotAction->LegMoveToRelatively(1, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(1, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(1, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(4, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(4, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(2, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(2, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(2, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(5, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(5, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(3, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(3, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(6, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(6, Point(0, 0, -stepHeight));

  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, 10));

  delay(300);

  // Then turn left
  robotAction->LegMoveToRelatively(1, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(1, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(1, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(4, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(4, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(2, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(2, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(2, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(5, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(5, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(3, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, legReach, 0));
  robotAction->LegMoveToRelatively(3, Point(0, 0, -stepHeight));

  robotAction->LegMoveToRelatively(6, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, -legReach, 0));
  robotAction->LegMoveToRelatively(6, Point(0, 0, -stepHeight));

  robotAction->TwistBody(Point(0, 0, 0), Point(0, 0, -10));

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
