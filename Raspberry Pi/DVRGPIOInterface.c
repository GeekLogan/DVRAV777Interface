#include <stdio.h>
#include <wiringPi.h>

#define OEPINCOUNT 4
#define BUTTONPINCOUNT 8
#define LIGHTCOUNT 23

#define CLOCKPIN 9
#define DATAPIN 8

int OEOut[] = {7,0,2,3};
int ButtonsIn[] = {12,13,14,11,10,6,5,4};
int status[BUTTONPINCOUNT * OEPINCOUNT];
int lightStatus[LIGHTCOUNT];

void writeLights() {
	digitalWrite(CLOCKPIN, LOW);
	for(int i = LIGHTCOUNT; i >= 0; i--) {
		digitalWrite(DATAPIN, lightStatus[i]);
		digitalWrite(CLOCKPIN, HIGH);
		digitalWrite(CLOCKPIN, LOW);
	}
}

void setLight(int pin, int in) {
	if(pin < 0 || pin >= LIGHTCOUNT) return;
	lightStatus[pin] = in;
	writeLights();
}

int mapLight(int id) {
	if(id >= 16) {
		if(id <= 18) return(26 - id);
		if(id <= 23) return(id - 8);
		if(id <= 26) return(26 - id);
		if(id <= 31) return(id - 24);
	}
	return -1;
}

void clearAllOE() {
	for(int i = 0; i < OEPINCOUNT; i++) 
		digitalWrite(OEOut[i], HIGH);
}

int mapkey(int oe, int qpin) {
	return (BUTTONPINCOUNT * oe) + qpin;
}

void updateGPIO() {
	for(int j = 0; j < OEPINCOUNT; j++) {
		clearAllOE();
		digitalWrite(OEOut[j], LOW);
		for(int k = 0; k < BUTTONPINCOUNT; k++) {
			if(k == 0) delayMicroseconds(1);
			int id = mapkey(j,k);
			int new = digitalRead(ButtonsIn[k]);
			
			if(status[id] != new) {
				status[id] = new;
				int lightId = mapLight(id);
				if(new) {
					printf("Button Up!  ");
					setLight(lightId, LOW);
				} else {
					printf("Button Down!");
					setLight(lightId, HIGH);
				}
				printf(" (%d) [%d] <", id, lightId);
				for(int i = 0; i < LIGHTCOUNT; i++) {
					printf("%d", lightStatus[i]);
				}
				printf("> \n");
			}
		}
	}
}

void setupGPIO() {
	wiringPiSetup();
	for(int i = 0; i < OEPINCOUNT; i++) {
		pinMode(OEOut[i], OUTPUT);
	}
	clearAllOE();
	for(int i = 0; i < BUTTONPINCOUNT; i++) {
		int pin = ButtonsIn[i];
		pinMode(pin, INPUT);
		pullUpDnControl(pin, PUD_DOWN);
	}
	pinMode(DATAPIN, OUTPUT);
	pinMode(CLOCKPIN, OUTPUT);
	for(int i = 0; i < BUTTONPINCOUNT * OEPINCOUNT; i++) {
		status[i] = HIGH;
	}
	for(int i = 0; i < LIGHTCOUNT; i++) {
		lightStatus[i] = LOW;
	}
	writeLights();
}

int main(int argc, char ** args) {
	setupGPIO();
	while(1) updateGPIO();
}
