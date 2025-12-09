/*
 * File       Idle Animation System for Project Damson Hexapod Robot
 * Brief      Core idle system - animations are in separate files:
 *            - IdleAnimSubtle.cpp     (Breathing, WeightShift, LookAround)
 *            - IdleAnimExpressive.cpp (Stretch, ShakeOff, Yawn, TapFoot)
 *            - IdleAnimPlayful.cpp    (Wave, DanceWiggle, CuriousPeek, HappyBounce)
 *            - IdleAnimDramatic.cpp   (Startle, PounceReady, VictoryPose)
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 * -----------------------------------------------------------------------------------------------*/

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonIdle.h"
#include <Arduino.h>

IdleAnimations::IdleAnimations() {}

void IdleAnimations::SetRobotAction(RobotAction* action)
{
  robotAction = action;

  // Seed random number generator with noise from an unconnected analog pin
  randomSeed(analogRead(A15));
}

void IdleAnimations::Update()
{
  if (!enabled || robotAction == nullptr) return;

  // Don't trigger new animations while one is running
  if (isAnimating) return;

  unsigned long currentTime = millis();
  unsigned long idleTime = currentTime - lastActivityTime;

  // Check if we've been idle long enough
  if (idleTime >= (unsigned long)timeoutSeconds * 1000) {
    isAnimating = true;

    // Pick a random weighted animation (won't repeat last one)
    int anim = PickRandomAnimation();
    lastAnimation = anim;

    // Play the selected animation
    switch (anim) {
      case 0:  Breathing(); break;
      case 1:  WeightShift(); break;
      case 2:  LookAround(); break;
      case 3:  Stretch(); break;
      case 4:  ShakeOff(); break;
      case 5:  Yawn(); break;
      case 6:  TapFoot(); break;
      case 7:  Wave(); break;
      case 8:  DanceWiggle(); break;
      case 9:  CuriousPeek(); break;
      case 10: HappyBounce(); break;
      case 11: Startle(); break;
      case 12: PounceReady(); break;
      case 13: VictoryPose(); break;
      case 14: DrumFingers(); break;
      case 15: StandTall(); break;
      case 16: LieDown(); break;
      default: Breathing(); break;
    }

    isAnimating = false;

    // Reset timer AFTER animation completes
    lastActivityTime = millis();

    // Randomize next timeout (weighted towards shorter delays)
    // Using squared random for weighting: more likely to be lower
    float r = random(1000) / 1000.0;  // 0.0 to 1.0
    r = r * r;                         // Square it to weight towards lower values
    timeoutSeconds = 1 + (int)(r * 4); // 1 to 5 seconds
  }
}

// Weighted random selection - subtle/short animations more likely
int IdleAnimations::PickRandomAnimation()
{
  // Weights for each animation (higher = more likely)
  // Subtle (0-2): high weight
  // Expressive (3-6): medium weight
  // Playful (7-10): medium weight
  // Dramatic (11-14): low weight
  static const int weights[] = {
    5,  // 0:  Breathing (subtle)
    5,  // 1:  WeightShift (subtle)
    5,  // 2:  LookAround (subtle)
    3,  // 3:  Stretch (expressive)
    3,  // 4:  ShakeOff (expressive, short)
    3,  // 5:  Yawn (expressive)
    3,  // 6:  TapFoot (expressive)
    2,  // 7:  Wave (playful)
    2,  // 8:  DanceWiggle (playful)
    2,  // 9:  CuriousPeek (playful)
    2,  // 10: HappyBounce (playful, short)
    1,  // 11: Startle (dramatic)
    1,  // 12: PounceReady (dramatic)
    1,  // 13: VictoryPose (dramatic)
    1,  // 14: DrumFingers (dramatic)
    1,  // 15: StandTall (dramatic)
    1   // 16: LieDown (dramatic)
  };

  // Calculate total weight (excluding last animation)
  int totalWeight = 0;
  for (int i = 0; i < ANIMATION_COUNT - 1; i++) {
    if (i != lastAnimation) {
      totalWeight += weights[i];
    }
  }

  // Pick random value in weight range
  int pick = random(totalWeight);

  // Find which animation that corresponds to
  int cumulative = 0;
  for (int i = 0; i < ANIMATION_COUNT - 1; i++) {
    if (i != lastAnimation) {
      cumulative += weights[i];
      if (pick < cumulative) {
        return i;
      }
    }
  }

  // Fallback (shouldn't reach here)
  return 0;
}

void IdleAnimations::ResetTimer()
{
  lastActivityTime = millis();
}

void IdleAnimations::SetTimeout(unsigned int seconds)
{
  timeoutSeconds = seconds;
}

void IdleAnimations::SetEnabled(bool value)
{
  enabled = value;
  if (enabled) {
    ResetTimer();
  }
}

bool IdleAnimations::IsEnabled()
{
  return enabled;
}

bool IdleAnimations::IsAnimating()
{
  return isAnimating;
}

void IdleAnimations::Reset()
{
  if (robotAction == nullptr) return;
  robotAction->InitialState();
}

#endif
