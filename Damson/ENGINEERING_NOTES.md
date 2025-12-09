# Damson Engineering Notes

This document captures implementation approaches, rationale, and learnings for the Damson hexapod. It complements the product-focused `Damson/PRD.md`.

## Movement Smoothing (Phase 1)

- Objective: Reduce jerkiness in servo motion, especially during leg lift/swing in walking gaits.
- Approach: Time‑based interpolation with easing in the core update loop (no API changes required).

### Design Overview

- Where: Implemented in `ProjectDamsonBasic.{h,cpp}` within `Robot::UpdateLegAction` and `RobotLeg` state.
- Trajectory State (per leg):
  - `pointStart`, `pointGoal`, `totalDistance`, `moveStartMillis`, `moveDurationMs`, `hasTrajectory`.
- Easing: Cubic S‑curve $e(t) = 3t^2 - 2t^3$ for smooth start/stop.
- Timing: Driven by `FlexiTimer2` at 20 ms; duration scales with prior step model so overall pace remains familiar.

### Why This vs. Fixed Step Increments

- Fixed increments cause visible discontinuities when step size or targets change.
- Eased time curves yield smoother accelerations without altering public APIs or higher‑level gait logic.

### Current Behavior

- New `MoveTo` resets trajectory. First tick initializes duration from distance and configured `stepDistance * speedMultiple`.
- Each tick computes eased progress and updates `MoveToDirectly` to the interpolated point.
- Movement finishes by snapping cleanly to exact goal and clearing `isBusy`.

### Tunables and Future Work

- Duration mapping: adjust how distance maps to `moveDurationMs` to speed up or slow down swings.
- Axis‑specific easing: apply stronger easing on Z (lift) than XY (swing) for additional “pop.”
- Alternative curves: test cosine ease or quintic $6t^5-15t^4+10t^3$ for even smoother profiles.
- Per‑gait profiles: different curves for crawl vs. turn vs. body twist.

## Testing & Verification

- Build: `pio run`
- Upload: `pio run -t upload` (ensure correct `upload_port`)
- Monitor: `pio device monitor -b 115200`

Checklist while testing:

- Legs accelerate/decelerate smoothly during lift/swing.
- Overall gait pace matches or improves prior timing.
- No stalls or oscillations at goal completion.
- Body twist and height changes remain responsive.

## Open Questions

- Should we expose an “easing on/off” flag for diagnostics?
- Preferred default curve and duration scaling per action group (2/4/6‑step)?
- Separate tuning for leg lift vs. swing distances?

## Changelog

- 2025‑12‑02: Introduced eased interpolation (cubic S‑curve) for leg trajectories; no API changes.
