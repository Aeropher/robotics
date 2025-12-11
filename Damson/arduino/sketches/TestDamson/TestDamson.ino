/*
 * TestDamson.ino
 *
 * Test sketch with serial command interface for tuning idle animations.
 *
 * Serial Commands (115200 baud):
 *   help        - List all commands
 *   breathing   - Play Breathing animation
 *   weightshift - Play WeightShift animation
 *   lookaround  - Play LookAround animation
 *   stretch     - Play Stretch animation
 *   shakeoff    - Play ShakeOff animation
 *   yawn        - Play Yawn animation
 *   tapfoot     - Play TapFoot animation
 *   wave        - Play Wave animation
 *   dance       - Play DanceWiggle animation
 *   curious     - Play CuriousPeek animation
 *   bounce      - Play HappyBounce animation
 *   startle     - Play Startle animation
 *   pounce      - Play PounceReady animation
 *   victory     - Play VictoryPose animation
 *   drum        - Play DrumFingers animation
 *   tall        - Play StandTall animation
 *   liedown     - Play LieDown animation
 *   reset       - Return to initial position
 *   idle on     - Enable auto idle animations
 *   idle off    - Disable auto idle animations
 *   timeout N   - Set idle timeout to N seconds
 */

#include <ProjectDamson.h>

ProjectDamson damson;
String inputBuffer = "";

void printHelp() {
  Serial.println(F(""));
  Serial.println(F("=== Damson Animation Tester ==="));
  Serial.println(F(""));
  Serial.println(F("Subtle animations:"));
  Serial.println(F("  breathing   - Gentle rise and fall"));
  Serial.println(F("  weightshift - Shift weight side to side"));
  Serial.println(F("  lookaround  - Tilt and look around"));
  Serial.println(F(""));
  Serial.println(F("Expressive animations:"));
  Serial.println(F("  stretch     - Cat-like stretch"));
  Serial.println(F("  shakeoff    - Shake off water"));
  Serial.println(F("  yawn        - Bow/yawn motion"));
  Serial.println(F("  tapfoot     - Impatient foot tap"));
  Serial.println(F(""));
  Serial.println(F("Playful animations:"));
  Serial.println(F("  wave        - Wave a leg"));
  Serial.println(F("  dance       - Dance wiggle"));
  Serial.println(F("  curious     - Curious peek"));
  Serial.println(F("  bounce      - Happy bounce"));
  Serial.println(F(""));
  Serial.println(F("Dramatic animations:"));
  Serial.println(F("  startle     - Startled crouch"));
  Serial.println(F("  pounce      - Pounce ready pose"));
  Serial.println(F("  victory     - Victory pose"));
  Serial.println(F("  drum        - Drum fingers"));
  Serial.println(F("  tall        - Stand tall"));
  Serial.println(F("  liedown     - Lie down and stretch"));
  Serial.println(F(""));
  Serial.println(F("Control:"));
  Serial.println(F("  reset       - Return to neutral"));
  Serial.println(F("  idle on     - Enable auto animations"));
  Serial.println(F("  idle off    - Disable auto animations"));
  Serial.println(F("  timeout N   - Set idle timeout (seconds)"));
  Serial.println(F(""));
  Serial.println(F("Motor Testing (bypasses limits!):"));
  Serial.println(F("  servo L J A - Set servo angle directly"));
  Serial.println(F("                L=leg(1-6) J=joint(0-2) A=angle(0-180)"));
  Serial.println(F("                Joint: 0=Hip, 1=Femur, 2=Tibia"));
  Serial.println(F(""));
}

void processCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();

  if (cmd.length() == 0) return;

  Serial.print(F("> "));
  Serial.println(cmd);

  // Subtle animations
  if (cmd == "breathing") {
    Serial.println(F("Playing: Breathing"));
    damson.idle.Breathing();
    Serial.println(F("Done."));
  }
  else if (cmd == "weightshift") {
    Serial.println(F("Playing: WeightShift"));
    damson.idle.WeightShift();
    Serial.println(F("Done."));
  }
  else if (cmd == "lookaround") {
    Serial.println(F("Playing: LookAround"));
    damson.idle.LookAround();
    Serial.println(F("Done."));
  }
  // Expressive animations
  else if (cmd == "stretch") {
    Serial.println(F("Playing: Stretch"));
    damson.idle.Stretch();
    Serial.println(F("Done."));
  }
  else if (cmd == "shakeoff") {
    Serial.println(F("Playing: ShakeOff"));
    damson.idle.ShakeOff();
    Serial.println(F("Done."));
  }
  else if (cmd == "yawn") {
    Serial.println(F("Playing: Yawn"));
    damson.idle.Yawn();
    Serial.println(F("Done."));
  }
  else if (cmd == "tapfoot") {
    Serial.println(F("Playing: TapFoot"));
    damson.idle.TapFoot();
    Serial.println(F("Done."));
  }
  // Playful animations
  else if (cmd == "wave") {
    Serial.println(F("Playing: Wave"));
    damson.idle.Wave();
    Serial.println(F("Done."));
  }
  else if (cmd == "dance") {
    Serial.println(F("Playing: DanceWiggle"));
    damson.idle.DanceWiggle();
    Serial.println(F("Done."));
  }
  else if (cmd == "curious") {
    Serial.println(F("Playing: CuriousPeek"));
    damson.idle.CuriousPeek();
    Serial.println(F("Done."));
  }
  else if (cmd == "bounce") {
    Serial.println(F("Playing: HappyBounce"));
    damson.idle.HappyBounce();
    Serial.println(F("Done."));
  }
  // Dramatic animations
  else if (cmd == "startle") {
    Serial.println(F("Playing: Startle"));
    damson.idle.Startle();
    Serial.println(F("Done."));
  }
  else if (cmd == "pounce") {
    Serial.println(F("Playing: PounceReady"));
    damson.idle.PounceReady();
    Serial.println(F("Done."));
  }
  else if (cmd == "victory") {
    Serial.println(F("Playing: VictoryPose"));
    damson.idle.VictoryPose();
    Serial.println(F("Done."));
  }
  else if (cmd == "drum") {
    Serial.println(F("Playing: DrumFingers"));
    damson.idle.DrumFingers();
    Serial.println(F("Done."));
  }
  else if (cmd == "tall") {
    Serial.println(F("Playing: StandTall"));
    damson.idle.StandTall();
    Serial.println(F("Done."));
  }
  else if (cmd == "liedown") {
    Serial.println(F("Playing: LieDown"));
    damson.idle.LieDown();
    Serial.println(F("Done."));
  }
  else if (cmd == "twitch") {
    Serial.println(F("Playing: AllLegTwitch"));
    damson.idle.AllLegTwitch();
    Serial.println(F("Done."));
  }
  else if (cmd == "defensive") {
    Serial.println(F("Playing: DefensiveCrouch"));
    damson.idle.DefensiveCrouch();
    Serial.println(F("Done."));
  }
  // Walk animations - Default Gait (Tripod)
  else if (cmd == "deffb") {
    Serial.println(F("Playing: DefaultForwardBack"));
    damson.idle.DefaultForwardBack();
    Serial.println(F("Done."));
  }
  else if (cmd == "defbf") {
    Serial.println(F("Playing: DefaultBackForward"));
    damson.idle.DefaultBackForward();
    Serial.println(F("Done."));
  }
  else if (cmd == "deflr") {
    Serial.println(F("Playing: DefaultLeftRight"));
    damson.idle.DefaultLeftRight();
    Serial.println(F("Done."));
  }
  else if (cmd == "defrl") {
    Serial.println(F("Playing: DefaultRightLeft"));
    damson.idle.DefaultRightLeft();
    Serial.println(F("Done."));
  }
  else if (cmd == "deftlr") {
    Serial.println(F("Playing: DefaultTurnLeftRight"));
    damson.idle.DefaultTurnLeftRight();
    Serial.println(F("Done."));
  }
  else if (cmd == "deftrl") {
    Serial.println(F("Playing: DefaultTurnRightLeft"));
    damson.idle.DefaultTurnRightLeft();
    Serial.println(F("Done."));
  }
  // Walk animations - Wave Gait (One leg at a time)
  else if (cmd == "wavefb") {
    Serial.println(F("Playing: WaveForwardBack"));
    damson.idle.WaveForwardBack();
    Serial.println(F("Done."));
  }
  else if (cmd == "wavebf") {
    Serial.println(F("Playing: WaveBackForward"));
    damson.idle.WaveBackForward();
    Serial.println(F("Done."));
  }
  else if (cmd == "wavelr") {
    Serial.println(F("Playing: WaveLeftRight"));
    damson.idle.WaveLeftRight();
    Serial.println(F("Done."));
  }
  else if (cmd == "waverl") {
    Serial.println(F("Playing: WaveRightLeft"));
    damson.idle.WaveRightLeft();
    Serial.println(F("Done."));
  }
  else if (cmd == "wavetlr") {
    Serial.println(F("Playing: WaveTurnLeftRight"));
    damson.idle.WaveTurnLeftRight();
    Serial.println(F("Done."));
  }
  else if (cmd == "wavetrl") {
    Serial.println(F("Playing: WaveTurnRightLeft"));
    damson.idle.WaveTurnRightLeft();
    Serial.println(F("Done."));
  }
  // Actions (not idle animations)
  else if (cmd == "jump") {
    Serial.println(F("Action: Jump (75%)"));
    damson.Jump();
    Serial.println(F("Done."));
  }
  else if (cmd == "jump50") {
    Serial.println(F("Action: Jump 50%"));
    damson.JumpWithSpeed(0.5);
    Serial.println(F("Done."));
  }
  else if (cmd == "jump75") {
    Serial.println(F("Action: Jump 75%"));
    damson.JumpWithSpeed(0.75);
    Serial.println(F("Done."));
  }
  else if (cmd == "jump100") {
    Serial.println(F("Action: Jump 100%"));
    damson.JumpWithSpeed(1.0);
    Serial.println(F("Done."));
  }
  // Control commands
  else if (cmd == "reset") {
    Serial.println(F("Resetting to initial position..."));
    damson.idle.Reset();
    Serial.println(F("Done."));
  }
  else if (cmd == "idle on") {
    damson.idle.SetEnabled(true);
    Serial.println(F("Idle animations ENABLED"));
  }
  else if (cmd == "idle off") {
    damson.idle.SetEnabled(false);
    Serial.println(F("Idle animations DISABLED"));
  }
  else if (cmd.startsWith("timeout ")) {
    int seconds = cmd.substring(8).toInt();
    if (seconds > 0) {
      damson.idle.SetTimeout(seconds);
      Serial.print(F("Idle timeout set to "));
      Serial.print(seconds);
      Serial.println(F(" seconds"));
    } else {
      Serial.println(F("Invalid timeout value"));
    }
  }
  // Motor testing: servo <leg> <joint> <angle>
  // leg: 1-6, joint: 0=A(hip), 1=B(femur), 2=C(tibia), angle: 0-180
  else if (cmd.startsWith("servo ")) {
    int firstSpace = 6;
    int secondSpace = cmd.indexOf(' ', firstSpace);
    int thirdSpace = cmd.indexOf(' ', secondSpace + 1);

    if (secondSpace > 0 && thirdSpace > 0) {
      int leg = cmd.substring(firstSpace, secondSpace).toInt();
      int joint = cmd.substring(secondSpace + 1, thirdSpace).toInt();
      int angle = cmd.substring(thirdSpace + 1).toInt();

      if (leg >= 1 && leg <= 6 && joint >= 0 && joint <= 2 && angle >= 0 && angle <= 180) {
        const char* jointNames[] = {"A (Hip)", "B (Femur)", "C (Tibia)"};
        Serial.print(F("Setting Leg "));
        Serial.print(leg);
        Serial.print(F(" Joint "));
        Serial.print(jointNames[joint]);
        Serial.print(F(" to "));
        Serial.print(angle);
        Serial.println(F(" degrees"));

        damson.SetServoAngle(leg, joint, angle);
        Serial.println(F("Done."));
      } else {
        Serial.println(F("Invalid values. Use: servo <leg 1-6> <joint 0-2> <angle 0-180>"));
      }
    } else {
      Serial.println(F("Usage: servo <leg> <joint> <angle>"));
      Serial.println(F("  leg: 1-6, joint: 0=A(hip), 1=B(femur), 2=C(tibia), angle: 0-180"));
    }
  }
  else if (cmd == "angles") {
    // Report current servo angles for all legs
    // Format: leg,hip,femur,tibia for each leg
    Serial.println(F("ANGLES:"));
    // Note: We don't have direct access to servoAngleNow from here
    // This would need to be added to the Robot class
    // For now, output placeholder that web interface can parse
    Serial.println(F("Not implemented yet - use reset to sync"));
  }
  else if (cmd == "help" || cmd == "?") {
    printHelp();
  }
  else {
    Serial.print(F("Unknown command: "));
    Serial.println(cmd);
    Serial.println(F("Type 'help' for commands"));
  }

  // Reset idle timer after any command
  damson.idle.ResetTimer();
}

void setup() {
  // Start with communication DISABLED so we can use serial for text commands
  // (The protocol handler would otherwise consume our serial data)
  damson.Start(false);

  // Manually start serial since we disabled commFunction
  Serial.begin(115200);

  // Signature startup: shake front-right leg to confirm Damson firmware
  damson.StartupShake(1, 3);  // Leg 1, shake 3 times

  // Enable idle animations (use web interface to disable for manual testing)
  damson.idle.SetEnabled(true);

  // Print welcome message
  Serial.println(F(""));
  Serial.println(F("Damson Animation Tester Ready"));
  Serial.println(F("Type 'help' for commands"));
  Serial.println(F(""));
}

void loop() {
  // Handle incoming commands from Serial/WiFi
  damson.Update();

  // Check for idle animations
  damson.idle.Update();

  // Read serial commands for animation testing
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}
