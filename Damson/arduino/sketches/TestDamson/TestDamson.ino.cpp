# 1 "C:\\Users\\ajmhe\\AppData\\Local\\Temp\\tmp81m23msn"
#include <Arduino.h>
# 1 "C:/Repos/robotics/Damson/arduino/sketches/TestDamson/TestDamson.ino"
# 31 "C:/Repos/robotics/Damson/arduino/sketches/TestDamson/TestDamson.ino"
#include <ProjectDamson.h>

ProjectDamson damson;
String inputBuffer = "";
void printHelp();
void processCommand(String cmd);
void setup();
void loop();
#line 36 "C:/Repos/robotics/Damson/arduino/sketches/TestDamson/TestDamson.ino"
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
}

void processCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();

  if (cmd.length() == 0) return;

  Serial.print(F("> "));
  Serial.println(cmd);


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
  else if (cmd == "help" || cmd == "?") {
    printHelp();
  }
  else {
    Serial.print(F("Unknown command: "));
    Serial.println(cmd);
    Serial.println(F("Type 'help' for commands"));
  }


  damson.idle.ResetTimer();
}

void setup() {


  damson.Start(false);


  Serial.begin(115200);


  damson.StartupShake(1, 3);


  damson.idle.SetEnabled(true);


  Serial.println(F(""));
  Serial.println(F("Damson Animation Tester Ready"));
  Serial.println(F("Type 'help' for commands"));
  Serial.println(F(""));
}

void loop() {

  damson.Update();


  damson.idle.Update();


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