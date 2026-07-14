
int s1 = 2;
int s2 = 3;
int s3 = 4;
int s4 = 5;

// ----------- LED PINS -----------
int r1_red = 10;
int r1_green = 11;

int r2_red = 8;
int r2_green = 9;

// ----------- TIMING -----------
unsigned long previousTime = 0;
unsigned long maxInterval = 30000;   // 30 sec max
unsigned long minGreen = 5000;       // 5 sec minimum
unsigned long baseTime = 30000;      // 10 sec guaranteed
unsigned long extraTime = 1000;      // 5 sec per vehicle

int currentRoad = 1;

// ----------- SETUP -----------
void setup() {
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);

  pinMode(r1_red, OUTPUT);
  pinMode(r1_green, OUTPUT);
  pinMode(r2_red, OUTPUT);
  pinMode(r2_green, OUTPUT);

  Serial.begin(9600);
}

// ----------- STABLE SENSOR FUNCTION -----------
int readStable(int pin) {
  int count = 0;
  for (int i = 0; i < 5; i++) {
    if (digitalRead(pin) == LOW) count++;
    delay(5);
  }
  return (count >= 3) ? 1 : 0;
}

// ----------- LOOP -----------
void loop() {

  int road1 = readStable(s1) + readStable(s2);
  int road2 = readStable(s3) + readStable(s4);

  unsigned long currentTime = millis();

  // Calculate dynamic time
  unsigned long greenTime1 = baseTime + (road1 * extraTime);
  unsigned long greenTime2 = baseTime + (road2 * extraTime);

  // Debug
  Serial.print("R1: ");
  Serial.print(road1);
  Serial.print("  R2: ");
  Serial.println(road2);

  // ----------- SWITCH LOGIC -----------

  if (currentRoad == 1) {

    // Stay until min time passed
    if ((currentTime - previousTime > minGreen)) {

      // Switch if other road has more vehicles
      if (road2 > road1) {
        currentRoad = 2;
        previousTime = currentTime;
      }

      // OR switch if time exceeded for this road
      else if (currentTime - previousTime >= greenTime1) {
        currentRoad = 2;
        previousTime = currentTime;
      }
    }

  } else {

    if ((currentTime - previousTime > minGreen)) {

      if (road1 > road2) {
        currentRoad = 1;
        previousTime = currentTime;
      }

      else if (currentTime - previousTime >= greenTime2) {
        currentRoad = 1;
        previousTime = currentTime;
      }
    }
  }

  // ----------- OUTPUT CONTROL -----------
  if (currentRoad == 1) {
    digitalWrite(r1_green, HIGH);
    digitalWrite(r1_red, LOW);

    digitalWrite(r2_green, LOW);
    digitalWrite(r2_red, HIGH);
  } 
  else {
    digitalWrite(r2_green, HIGH);
    digitalWrite(r2_red, LOW);

    digitalWrite(r1_green, LOW);
    digitalWrite(r1_red, HIGH);
  }

  delay(200);
}
