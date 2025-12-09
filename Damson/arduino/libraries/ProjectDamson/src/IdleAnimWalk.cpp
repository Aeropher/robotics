/*
 * File       Walk Idle Animations
 * Brief      WalkForwardBack, WalkLeftRight, WalkTurnLeftRight
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonIdle.h"
#include <Arduino.h>

// WalkForwardBack: 2 steps forward, pause, 2 steps back
void IdleAnimations::WalkForwardBack()
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

// WalkLeftRight: 2 steps left, pause, 2 steps right
void IdleAnimations::WalkLeftRight()
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

// WalkTurnLeftRight: Turn left, pause, turn right
void IdleAnimations::WalkTurnLeftRight()
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

// TripodForwardBack: Tripod gait - move 3 legs at once
// Group A: legs 1, 3, 5 (front-right, back-right, mid-left)
// Group B: legs 2, 4, 6 (mid-right, front-left, back-left)
void IdleAnimations::TripodForwardBack()
{
  if (robotAction == nullptr) return;

  const float stepHeight = 25;   // mm - lift legs
  const float stepLength = 30;   // mm - forward/back distance

  // === Step 1: Move forward ===
  // Lift Group A legs
  robotAction->LegMoveToRelatively(1, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, 0, stepHeight));

  // Move Group A forward, Group B backward (body moves forward)
  robotAction->LegMoveToRelatively(1, Point(0, stepLength, 0));
  robotAction->LegMoveToRelatively(3, Point(0, stepLength, 0));
  robotAction->LegMoveToRelatively(5, Point(0, stepLength, 0));
  robotAction->LegMoveToRelatively(2, Point(0, -stepLength, 0));
  robotAction->LegMoveToRelatively(4, Point(0, -stepLength, 0));
  robotAction->LegMoveToRelatively(6, Point(0, -stepLength, 0));

  // Lower Group A
  robotAction->LegMoveToRelatively(1, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, 0, -stepHeight));

  // === Step 2: Continue forward ===
  // Lift Group B legs
  robotAction->LegMoveToRelatively(2, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, 0, stepHeight));

  // Move Group B forward, Group A backward
  robotAction->LegMoveToRelatively(2, Point(0, stepLength * 2, 0));
  robotAction->LegMoveToRelatively(4, Point(0, stepLength * 2, 0));
  robotAction->LegMoveToRelatively(6, Point(0, stepLength * 2, 0));
  robotAction->LegMoveToRelatively(1, Point(0, -stepLength, 0));
  robotAction->LegMoveToRelatively(3, Point(0, -stepLength, 0));
  robotAction->LegMoveToRelatively(5, Point(0, -stepLength, 0));

  // Lower Group B
  robotAction->LegMoveToRelatively(2, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, 0, -stepHeight));

  delay(300);

  // === Step 3: Move back ===
  // Lift Group A
  robotAction->LegMoveToRelatively(1, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, 0, stepHeight));

  // Move Group A backward, Group B forward
  robotAction->LegMoveToRelatively(1, Point(0, -stepLength, 0));
  robotAction->LegMoveToRelatively(3, Point(0, -stepLength, 0));
  robotAction->LegMoveToRelatively(5, Point(0, -stepLength, 0));
  robotAction->LegMoveToRelatively(2, Point(0, stepLength, 0));
  robotAction->LegMoveToRelatively(4, Point(0, stepLength, 0));
  robotAction->LegMoveToRelatively(6, Point(0, stepLength, 0));

  // Lower Group A
  robotAction->LegMoveToRelatively(1, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(3, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(5, Point(0, 0, -stepHeight));

  // === Step 4: Continue back ===
  // Lift Group B
  robotAction->LegMoveToRelatively(2, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, 0, stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, 0, stepHeight));

  // Move Group B backward, Group A forward
  robotAction->LegMoveToRelatively(2, Point(0, -stepLength * 2, 0));
  robotAction->LegMoveToRelatively(4, Point(0, -stepLength * 2, 0));
  robotAction->LegMoveToRelatively(6, Point(0, -stepLength * 2, 0));
  robotAction->LegMoveToRelatively(1, Point(0, stepLength, 0));
  robotAction->LegMoveToRelatively(3, Point(0, stepLength, 0));
  robotAction->LegMoveToRelatively(5, Point(0, stepLength, 0));

  // Lower Group B
  robotAction->LegMoveToRelatively(2, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(4, Point(0, 0, -stepHeight));
  robotAction->LegMoveToRelatively(6, Point(0, 0, -stepHeight));

  robotAction->InitialState();
}

#endif
