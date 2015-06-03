#define LEDPIN 13
#define DATA 12
#define CLOCK 11
#define STROBE A0
#define BITWIDTH 23
#define OECOUNT 4
#define BUTTONCOUNT 8

int registerData[BITWIDTH];
int buttonStatus[BUTTONCOUNT * OECOUNT];

int oepins[] = {A5, A4, A3, A2};
int buttonpins[] = {3, 4, 5, 6, 7, 8, 9, 10};

void setup() {
  pinMode(LEDPIN, OUTPUT); //LED Pin
  pinMode(DATA, OUTPUT);  
  pinMode(CLOCK, OUTPUT);
  pinMode(STROBE, OUTPUT);
  
  for(int i = 0; i < OECOUNT; i++) {
    pinMode(oepins[i], OUTPUT);
  }
  
  for(int i = 0; i < BUTTONCOUNT; i++) {
    pinMode(buttonpins[i], INPUT);
  }
  
  for(int i = 0; i < BUTTONCOUNT * OECOUNT; i++) {
    buttonStatus[i] = HIGH;
  }
  
  digitalWrite(LEDPIN, HIGH);
  digitalWrite(CLOCK, LOW);
  digitalWrite(STROBE, HIGH);
  
  clearAllOE();
  updateButtons();
  sendToRegister();
  
  Serial.begin(9600);
  delay(500);
}

int mapButton(int oe, int button) {
  return ((oe * BUTTONCOUNT) + button);
}

void sendToRegister() {
  for(byte i = 0; i < BITWIDTH; i = i + 1) {
    digitalWrite(DATA, registerData[i]);
    digitalWrite(CLOCK, HIGH);
    digitalWrite(CLOCK, LOW);
  } 
}

void checkCache() {
  if (Serial.available() >= BITWIDTH) {
    for(int i = 0; i < BITWIDTH; i = i + 1) {
      byte in = Serial.read();
      registerData[i] = (in == 49);
    }
    sendToRegister();
  }
}

void clearAllOE() {
  for(int i = 0; i < OECOUNT; i++) {
    digitalWrite(oepins[i], HIGH);
  }
}

void updateButtons() {
  for(int k = 0; k < OECOUNT; k++) {
    clearAllOE();
    digitalWrite(oepins[k], LOW);
    delay(25);
    for(int j = 0; j < BUTTONCOUNT; j++) {
      int newData = digitalRead(buttonpins[j]);
      int index = mapButton(k, j);
      
      if(buttonStatus[index] != newData) {
        buttonStatus[index] = newData;
        if(newData) {
          Serial.print("Button Up   [");
        } else {
          Serial.print("Button Down [");
        }
        Serial.print(index);
        Serial.println("]");
      }
    }
  }
}

void loop() { 
  checkCache();
  updateButtons();
}

