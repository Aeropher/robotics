/*
 * File       Motor limits and constraints for Project Damson
 * Project    Project Damson
 * Brief      Defines physical servo limits and inter-joint constraints
 * License    Creative Commons Attribution ShareAlike 3.0
 *            (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

#pragma once

// =============================================================================
// GLOBAL SERVO LIMITS
// =============================================================================
// MG90S servos are rated for 0-180 degrees, but in practice they typically
// only achieve about 170 degrees of rotation. These global limits provide
// a safe buffer to prevent servo strain at the mechanical end stops.

namespace GlobalServoLimits {
  constexpr int SERVO_MIN = 10;   // Safe minimum angle (degrees)
  constexpr int SERVO_MAX = 170;  // Safe maximum angle (degrees)

  // Helper to clamp an angle to safe range
  inline int clamp(int angle) {
    if (angle < SERVO_MIN) return SERVO_MIN;
    if (angle > SERVO_MAX) return SERVO_MAX;
    return angle;
  }
}

/*
 * SERVO ANGLE LIMITS
 *
 * These are the physical limits of each servo within the global safe range.
 * Values can be further restricted based on mechanical constraints of the robot.
 * Values determined by manual testing - moving each servo until binding occurs.
 *
 * Joint naming:
 *   A = Hip (horizontal rotation)
 *   B = Femur (upper leg, vertical)
 *   C = Tibia (lower leg, vertical)
 *
 * Leg layout (top view):
 *        FRONT
 *     1 ------- 4
 *     2 ------- 5
 *     3 ------- 6
 *        REAR
 *
 *   1 = Front Right    4 = Front Left
 *   2 = Middle Right   5 = Middle Left
 *   3 = Rear Right     6 = Rear Left
 */

struct ServoLimit {
  int min;  // Minimum servo angle (within GlobalServoLimits range)
  int max;  // Maximum servo angle (within GlobalServoLimits range)
};

struct LegLimits {
  ServoLimit hip;     // Joint A - horizontal rotation
  ServoLimit femur;   // Joint B - upper leg
  ServoLimit tibia;   // Joint C - lower leg
};

// =============================================================================
// PHYSICAL SERVO LIMITS (per leg)
// =============================================================================
// Default to global limits. Override with tighter values based on testing.
// Format: { min, max } for each joint

namespace ServoLimits {

  using namespace GlobalServoLimits;

  // Leg 1 - Front Right
  constexpr LegLimits leg1 = {
    .hip   = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .femur = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .tibia = { SERVO_MIN, SERVO_MAX }    // TODO: Test and update
  };

  // Leg 2 - Middle Right
  constexpr LegLimits leg2 = {
    .hip   = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .femur = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .tibia = { SERVO_MIN, SERVO_MAX }    // TODO: Test and update
  };

  // Leg 3 - Rear Right
  constexpr LegLimits leg3 = {
    .hip   = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .femur = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .tibia = { SERVO_MIN, SERVO_MAX }    // TODO: Test and update
  };

  // Leg 4 - Front Left
  constexpr LegLimits leg4 = {
    .hip   = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .femur = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .tibia = { SERVO_MIN, SERVO_MAX }    // TODO: Test and update
  };

  // Leg 5 - Middle Left
  constexpr LegLimits leg5 = {
    .hip   = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .femur = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .tibia = { SERVO_MIN, SERVO_MAX }    // TODO: Test and update
  };

  // Leg 6 - Rear Left
  constexpr LegLimits leg6 = {
    .hip   = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .femur = { SERVO_MIN, SERVO_MAX },   // TODO: Test and update
    .tibia = { SERVO_MIN, SERVO_MAX }    // TODO: Test and update
  };

  // Helper to get limits by leg number (1-6)
  inline const LegLimits& getLeg(int leg) {
    switch (leg) {
      case 1: return leg1;
      case 2: return leg2;
      case 3: return leg3;
      case 4: return leg4;
      case 5: return leg5;
      case 6: return leg6;
      default: return leg1;
    }
  }

}  // namespace ServoLimits


// =============================================================================
// INTER-JOINT CONSTRAINTS
// =============================================================================
// These define how one joint's position affects another joint's valid range.
//
// Example: When hip is rotated inward, femur may have reduced range to avoid
// collision with the body or adjacent legs.

namespace JointConstraints {

  /*
   * Constraint: How hip angle affects femur range
   *
   * When hip is at certain angles, the femur's valid range may be restricted.
   * Returns adjusted min/max for femur based on current hip angle.
   */
  struct FemurConstraint {
    int hipThreshold;      // Hip angle at which constraint applies
    bool whenHipBelow;     // true = constraint applies when hip < threshold
                           // false = constraint applies when hip > threshold
    int femurMinAdjust;    // Adjustment to femur min (positive = more restrictive)
    int femurMaxAdjust;    // Adjustment to femur max (negative = more restrictive)
  };

  /*
   * Constraint: How hip angle affects tibia range
   */
  struct TibiaConstraint {
    int hipThreshold;
    bool whenHipBelow;
    int tibiaMinAdjust;
    int tibiaMaxAdjust;
  };

  /*
   * Constraint: How femur angle affects tibia range
   *
   * When femur is raised high, tibia may need to be restricted to avoid
   * hitting the body or ground.
   */
  struct TibiaFromFemurConstraint {
    int femurThreshold;
    bool whenFemurBelow;
    int tibiaMinAdjust;
    int tibiaMaxAdjust;
  };

  // -------------------------------------------------------------------------
  // LEG 1 (Front Right) Constraints
  // -------------------------------------------------------------------------
  // TODO: Fill in from testing

  namespace Leg1 {
    // Femur constraints based on hip position
    constexpr FemurConstraint femurFromHip[] = {
      // { hipThreshold, whenHipBelow, femurMinAdjust, femurMaxAdjust }
      // Example: { 45, true, 10, 0 }  // When hip < 45, femur min += 10
    };
    constexpr int femurFromHipCount = 0;  // TODO: Update when adding constraints

    // Tibia constraints based on hip position
    constexpr TibiaConstraint tibiaFromHip[] = {
      // { hipThreshold, whenHipBelow, tibiaMinAdjust, tibiaMaxAdjust }
    };
    constexpr int tibiaFromHipCount = 0;

    // Tibia constraints based on femur position
    constexpr TibiaFromFemurConstraint tibiaFromFemur[] = {
      // { femurThreshold, whenFemurBelow, tibiaMinAdjust, tibiaMaxAdjust }
    };
    constexpr int tibiaFromFemurCount = 0;
  }

  // -------------------------------------------------------------------------
  // LEG 2 (Middle Right) Constraints
  // -------------------------------------------------------------------------
  namespace Leg2 {
    constexpr FemurConstraint femurFromHip[] = {};
    constexpr int femurFromHipCount = 0;
    constexpr TibiaConstraint tibiaFromHip[] = {};
    constexpr int tibiaFromHipCount = 0;
    constexpr TibiaFromFemurConstraint tibiaFromFemur[] = {};
    constexpr int tibiaFromFemurCount = 0;
  }

  // -------------------------------------------------------------------------
  // LEG 3 (Rear Right) Constraints
  // -------------------------------------------------------------------------
  namespace Leg3 {
    constexpr FemurConstraint femurFromHip[] = {};
    constexpr int femurFromHipCount = 0;
    constexpr TibiaConstraint tibiaFromHip[] = {};
    constexpr int tibiaFromHipCount = 0;
    constexpr TibiaFromFemurConstraint tibiaFromFemur[] = {};
    constexpr int tibiaFromFemurCount = 0;
  }

  // -------------------------------------------------------------------------
  // LEG 4 (Front Left) Constraints
  // -------------------------------------------------------------------------
  namespace Leg4 {
    constexpr FemurConstraint femurFromHip[] = {};
    constexpr int femurFromHipCount = 0;
    constexpr TibiaConstraint tibiaFromHip[] = {};
    constexpr int tibiaFromHipCount = 0;
    constexpr TibiaFromFemurConstraint tibiaFromFemur[] = {};
    constexpr int tibiaFromFemurCount = 0;
  }

  // -------------------------------------------------------------------------
  // LEG 5 (Middle Left) Constraints
  // -------------------------------------------------------------------------
  namespace Leg5 {
    constexpr FemurConstraint femurFromHip[] = {};
    constexpr int femurFromHipCount = 0;
    constexpr TibiaConstraint tibiaFromHip[] = {};
    constexpr int tibiaFromHipCount = 0;
    constexpr TibiaFromFemurConstraint tibiaFromFemur[] = {};
    constexpr int tibiaFromFemurCount = 0;
  }

  // -------------------------------------------------------------------------
  // LEG 6 (Rear Left) Constraints
  // -------------------------------------------------------------------------
  namespace Leg6 {
    constexpr FemurConstraint femurFromHip[] = {};
    constexpr int femurFromHipCount = 0;
    constexpr TibiaConstraint tibiaFromHip[] = {};
    constexpr int tibiaFromHipCount = 0;
    constexpr TibiaFromFemurConstraint tibiaFromFemur[] = {};
    constexpr int tibiaFromFemurCount = 0;
  }

}  // namespace JointConstraints


// =============================================================================
// LEG-TO-LEG COLLISION CONSTRAINTS
// =============================================================================
// These define when adjacent legs might collide based on their hip positions.

namespace LegCollisionConstraints {

  /*
   * Collision zone between two adjacent legs
   *
   * When both legs' hips are within their respective ranges, collision is possible.
   * The constraint specifies how to adjust one leg's range when the other is in
   * a certain position.
   */
  struct LegPairConstraint {
    int leg1;              // First leg number (1-6)
    int leg2;              // Second leg number (1-6)

    // When leg1's hip is in this range...
    int leg1HipMin;
    int leg1HipMax;

    // ...and leg2's hip is in this range...
    int leg2HipMin;
    int leg2HipMax;

    // ...collision is possible. Restrict leg2's hip to:
    int leg2HipSafeMin;
    int leg2HipSafeMax;
  };

  // TODO: Fill in from testing
  // Adjacent leg pairs that can collide:
  //   - Leg 1 (Front Right) <-> Leg 2 (Middle Right)
  //   - Leg 2 (Middle Right) <-> Leg 3 (Rear Right)
  //   - Leg 4 (Front Left) <-> Leg 5 (Middle Left)
  //   - Leg 5 (Middle Left) <-> Leg 6 (Rear Left)
  //   - Leg 1 (Front Right) <-> Leg 4 (Front Left) - front legs crossing

  constexpr LegPairConstraint collisionZones[] = {
    // { leg1, leg2, leg1HipMin, leg1HipMax, leg2HipMin, leg2HipMax, leg2SafeMin, leg2SafeMax }
    // Example: { 1, 2, 0, 45, 135, 180, 100, 180 }  // When leg1 hip is 0-45 and leg2 is 135-180, restrict leg2 to 100-180
  };
  constexpr int collisionZoneCount = 0;  // TODO: Update when adding constraints

}  // namespace LegCollisionConstraints


// =============================================================================
// BODY MOVEMENT LIMITS
// =============================================================================
// Safe ranges for coordinated body movements (TwistBody, etc.)

namespace BodyLimits {

  // Translation limits (mm)
  constexpr int moveXMin = -30;   // TODO: Test and update
  constexpr int moveXMax = 30;
  constexpr int moveYMin = -30;
  constexpr int moveYMax = 30;
  constexpr int moveZMin = 0;     // Can't go below ground
  constexpr int moveZMax = 45;    // TODO: Test and update - max height

  // Rotation limits (degrees)
  constexpr int rotateXMin = -15;  // Pitch backward
  constexpr int rotateXMax = 15;   // Pitch forward
  constexpr int rotateYMin = -15;  // Roll left
  constexpr int rotateYMax = 15;   // Roll right
  constexpr int rotateZMin = -15;  // Yaw left
  constexpr int rotateZMax = 15;   // Yaw right

}  // namespace BodyLimits
