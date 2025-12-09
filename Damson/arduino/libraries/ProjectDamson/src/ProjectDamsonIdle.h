/*
 * File       Idle Animations for Project Damson Hexapod Robot
 * Brief      Video game-style idle animations that play after periods of inactivity.
 *            Like a character select screen, the robot will cycle through animations
 *            when no input is received.
 *
 * Project    Project Damson
 * License    Creative Commons Attribution ShareAlike 3.0
 *            (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

#pragma once

#if defined(ARDUINO_AVR_MEGA2560)

#include "ProjectDamsonBasic.h"

class IdleAnimations
{
public:
  IdleAnimations();

  // Set the robot action reference (must be called before use)
  void SetRobotAction(RobotAction* action);

 /*
  * Brief     Update idle animation system - call this in loop()
  *           Tracks time since last activity and triggers animations when idle
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void Update();

 /*
  * Brief     Reset the idle timer (call when robot receives input/activity)
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void ResetTimer();

 /*
  * Brief     Set how long to wait before starting idle animations
  * Param     seconds   Idle time before animations start (default 20)
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void SetTimeout(unsigned int seconds);

 /*
  * Brief     Enable or disable idle animations
  * Param     enabled   true to enable, false to disable
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void SetEnabled(bool enabled);

 /*
  * Brief     Check if idle animations are enabled
  * Retval    true if enabled
  * -----------------------------------------------------------------------------------------------*/
  bool IsEnabled();

 /*
  * Brief     Check if an animation is currently running
  * Retval    true if animating
  * -----------------------------------------------------------------------------------------------*/
  bool IsAnimating();

 /*
  * Brief     Return to neutral/initial position
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void Reset();

  // ===============================================================================================
  // ANIMATIONS - Can also be called directly for testing
  // ===============================================================================================

  // --- Subtle/Ambient Animations ---

 /*
  * Brief     Gentle body rise and fall, like breathing
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void Breathing();

 /*
  * Brief     Slowly shift body weight side to side
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void WeightShift();

 /*
  * Brief     Tilt body as if looking around the environment
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void LookAround();

  // --- Expressive Animations ---

 /*
  * Brief     Extend legs outward one at a time, like a cat stretching
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void Stretch();

 /*
  * Brief     Quick side-to-side wiggle, like shaking off water
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void ShakeOff();

 /*
  * Brief     Lower front, raise back, then return - like a yawn or bow
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void Yawn();

 /*
  * Brief     One leg taps impatiently on the ground
  * Param     leg       Which leg to tap (1-6), default front-right
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void TapFoot(int leg = 1);

  // --- Playful Animations ---

 /*
  * Brief     Lift one front leg and wave it
  * Param     leg       Which leg to wave (1-6), default front-right
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void Wave(int leg = 1);

 /*
  * Brief     Rhythmic left-right body sway, like dancing
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void DanceWiggle();

 /*
  * Brief     Lean forward and tilt body side to side curiously
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void CuriousPeek();

 /*
  * Brief     Quick up-down bounce motion on all legs
  * Param     count     Number of bounces (default 3)
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void HappyBounce(int count = 3);

  // --- Alert/Dramatic Animations ---

 /*
  * Brief     Quick crouch then slowly rise back up, like being startled
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void Startle();

 /*
  * Brief     Lower front, raise rear - stalking/pounce ready pose
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void PounceReady();

 /*
  * Brief     Rise up tall, maybe with front legs raised - victory!
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void VictoryPose();

 /*
  * Brief     Tap each foot in sequence like drumming fingers on a table
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void DrumFingers();

 /*
  * Brief     Stand up as tall as possible
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void StandTall();

 /*
  * Brief     Lie down with body on the surface
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void LieDown();

 /*
  * Brief     Twitch all legs outward then back
  * Param     None
  * Retval    None
  * -----------------------------------------------------------------------------------------------*/
  void AllLegTwitch();

private:
  RobotAction* robotAction = nullptr;
  unsigned long lastActivityTime = 0;
  unsigned int timeoutSeconds = 2;  // TODO: Change back to 20 for production
  bool enabled = true;
  bool isAnimating = false;  // True while an animation is running
  int lastAnimation = -1;    // Last played animation (-1 = none)

  // Pick a weighted random animation (subtle/short more likely)
  int PickRandomAnimation();

  static const int ANIMATION_COUNT = 18;
};

#endif
