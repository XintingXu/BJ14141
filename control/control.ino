//define input analog pin
#define AUTO_CONTROL_PIN A1
#define AUTO_BE_COLD_PIN 2
#define AUTO_BE_HOT_PIN 3
#define HAND_BE_COLD_PIN 4
#define HAND_BE_HOT_PIN 5
#define HAND_BE_VET_PIN 6

//define output pin
#define COLD_TEC_1 7
#define COLD_TEC_2 8
#define HOT_TEC 9
#define COLD_FAN_1 10
#define COLD_FAN_2 11
#define HOT_FAN 12
#define VET 13

int Cold1WorkCount = 0;
int Cold2WorkCount = 1;
int HotWorkCount = 0;

bool AutoControl = true;
bool ColdWork = false;
bool HotWork = false;
bool Vetwork = false;

void setVet();
void setHot();
void setCold();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9200);
  pinMode(AUTO_CONTROL_PIN, INPUT);
  pinMode(AUTO_BE_COLD_PIN, INPUT);
  pinMode(AUTO_BE_HOT_PIN, INPUT);
  pinMode(HAND_BE_COLD_PIN, INPUT);
  pinMode(HAND_BE_HOT_PIN, INPUT);
  pinMode(HAND_BE_VET_PIN, INPUT);
  pinMode(COLD_TEC_1, OUTPUT); digitalWrite(COLD_TEC_1, HIGH);
  pinMode(COLD_TEC_2, OUTPUT); digitalWrite(COLD_TEC_2, HIGH);
  pinMode(HOT_TEC, OUTPUT); digitalWrite(HOT_TEC, HIGH);
  pinMode(COLD_FAN_1, OUTPUT); digitalWrite(COLD_FAN_1, HIGH);
  pinMode(COLD_FAN_2, OUTPUT); digitalWrite(COLD_FAN_2, HIGH);
  pinMode(HOT_FAN, OUTPUT); digitalWrite(HOT_FAN, HIGH);
  pinMode(VET, OUTPUT); digitalWrite(VET, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  int pin_data = digitalRead(AUTO_CONTROL_PIN);
  if (pin_data == 0) {
    AutoControl = true;
  } else {
    AutoControl = false;
  }
  setVet();
  setCold();
  setHot();
  delay(1000);
}

void setCold() {
  if (AutoControl == true) {
    if (digitalRead((AUTO_BE_COLD_PIN) == 0) || (digitalRead(HAND_BE_COLD_PIN) == 0)) {
      ColdWork = true;
    } else {
      ColdWork = false;
    }
  } else {
    if (digitalRead(HAND_BE_COLD_PIN) == 0) {
      ColdWork = true;
    } else if (digitalRead(HAND_BE_COLD_PIN) == 1) {
      ColdWork = false;
    } else ;
  }
  Serial.print("ColdWork = ");
  Serial.println(ColdWork);
  if (ColdWork == true) {
    digitalWrite(COLD_FAN_2, LOW);
    if ((Cold2WorkCount >= 0) && (Cold2WorkCount % 2 == 0)) {
      digitalWrite(COLD_FAN_1, HIGH);
      digitalWrite(COLD_TEC_2, LOW);
      digitalWrite(COLD_TEC_1, HIGH);
      Cold2WorkCount ++;
      Serial.println("Cold2Working....");
    } else;
    if ((Cold1WorkCount >= 0) && (Cold1WorkCount % 2 == 0)) {
      digitalWrite(COLD_FAN_1, HIGH);
      digitalWrite(COLD_TEC_1, LOW);
      digitalWrite(COLD_TEC_2, HIGH);
      Cold1WorkCount ++;
      Serial.println("Cold1Working....");
    } else;
    if (Cold2WorkCount < 0) {
      Cold2WorkCount ++;
    } else;
    if (Cold1WorkCount < 0) {
      Cold1WorkCount ++;
    } else;
    if (Cold2WorkCount >= 5) {
      digitalWrite(COLD_FAN_2, LOW);
      digitalWrite(COLD_TEC_1, HIGH);
      digitalWrite(COLD_TEC_2, HIGH);
      Cold1WorkCount = -4;
      Cold2WorkCount = -3;
      Serial.println("Colding....");
    } else;
  }
  else {
    digitalWrite(COLD_TEC_1, HIGH);
    digitalWrite(COLD_TEC_2, HIGH);
    digitalWrite(COLD_FAN_1, HIGH);
    digitalWrite(COLD_FAN_2, HIGH);
    Cold1WorkCount = 0;
    Cold2WorkCount = 1;
  }
}

void setHot() {
  if (AutoControl == true) {
    if (digitalRead(AUTO_BE_HOT_PIN) == 0 || digitalRead(HAND_BE_HOT_PIN) == 0) {
      HotWork = true;
    } else {
      HotWork = false;
    }
  } else {
    if (digitalRead(HAND_BE_HOT_PIN) == 0) {
      HotWork = true;
    } else if (digitalRead(HAND_BE_HOT_PIN) == 1) {
      HotWork = false;
    } else ;
  }
  Serial.print("HotWork = ");
  Serial.println(HotWork);
  if (HotWork == true) {
    digitalWrite(HOT_FAN, LOW);
    if ((HotWorkCount > 0) && (HotWorkCount < 5)) {
      digitalWrite(HOT_TEC, LOW);
      HotWorkCount ++;
      Serial.println("HotWorking....");
    }
    else {
      if (HotWorkCount < 0) {
        digitalWrite(HOT_TEC, HIGH);
        HotWorkCount ++;
      } else ;
      if (HotWorkCount >= 5) {
        HotWorkCount = -5;
        digitalWrite(HOT_TEC, HIGH);
        Serial.println("HotSleeping....");
      } else;
    }
  }
  else {
    digitalWrite(HOT_TEC, HIGH);
    digitalWrite(HOT_FAN, HIGH);
  }
}

void setVet() {
  if (digitalRead(HAND_BE_VET_PIN) == 0) {
    digitalWrite(VET, LOW);
  }
  else if (digitalRead(HAND_BE_VET_PIN) == 1) {
    digitalWrite(VET, HIGH);
  } else ;
}

