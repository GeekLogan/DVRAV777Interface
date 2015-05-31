#define LEDPIN 13
#define STROBE 12
#define DATA 11
#define CLOCK 10
#define BITWIDTH 23


boolean registerData[23];

boolean regOne[8];
boolean regTwo[8];
boolean regThree[8];
boolean regFour[8];

int lastCount = 0;

// the setup function runs once when you press reset or power the board
void setup() {
 pinMode(LEDPIN, OUTPUT); //LED Pin
 pinMode(STROBE, OUTPUT);
 pinMode(DATA, OUTPUT);  
 pinMode(CLOCK, OUTPUT);
 
 digitalWrite(STROBE, HIGH);
 digitalWrite(DATA, LOW);
 digitalWrite(LEDPIN, LOW);
 
 for(int i = 0; i < BITWIDTH; i = i + 1) {
   digitalWrite(CLOCK, HIGH);
   digitalWrite(CLOCK, LOW);
 }
 
 Serial.begin(9600);

}

void sendToRegister() { 
 for(byte i = 0; i < BITWIDTH; i = i + 1) {
  if(registerData[i]) {
    digitalWrite(DATA, HIGH);
  } else {
    digitalWrite(DATA, LOW);
  }
  
  delay(2);
  digitalWrite(CLOCK, HIGH);
  delay(2);
  digitalWrite(CLOCK, LOW);
  delay(2);
 } 
}

void printRegisters() { 
 Serial.print("Data: ");
 for(int i = 0; i < BITWIDTH; i = i + 1) {
   if(registerData[i]) {
     Serial.print(1);
   } else {
     Serial.print(0);
   }
 } 
 Serial.println();
}

// the loop function runs over and over again forever
void loop() {
  int count = Serial.available();

  if(count != lastCount) { 
    Serial.print("Buffer Count: ");
    Serial.println(count);
    lastCount = count;
  }
 
  if (count >= BITWIDTH) {
    for(int i = 0; i < BITWIDTH; i = i + 1) {
      byte in = Serial.read();
      Serial.print("Byte Read: ");
      Serial.println(in);
      registerData[i] = (in == 49);
    }
    printRegisters();
    sendToRegister();
  }
 
}
