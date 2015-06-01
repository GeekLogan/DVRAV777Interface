/*
gpio readall

gpio mode 7 output
gpio mode 0 output
gpio mode 2 output
gpio mode 3 output

gpio write 7 1
gpio write 0 1
gpio write 2 1
gpio write 3 1

gpio mode 12 input
gpio mode 13 input
gpio mode 14 input
gpio mode 11 input
gpio mode 10 input
gpio mode 6 input
gpio mode 5 input
gpio mode 4 input

gpio readall
*/

#include <stdio.h>
#include <wiringPi.h>

#define OEPINCOUNT 4
#define BUTTONPINCOUNT 8

int OEOut[] = {7,0,2,3};
int ButtonsIn[] = {12,13,14,11,10,6,5,4};

void clearAllOE() {
	int i;
	for(i = 0; i < OEPINCOUNT; i++) {
		int pin = OEOut[i];
		digitalWrite(pin, HIGH);
	}
}

void setupGPIO() {
	wiringPiSetup();

	int i;
	for(i = 0; i < OEPINCOUNT; i++) {
		int pin = OEOut[i];
		pinMode(pin, OUTPUT);
	}

	clearAllOE();

	for(i = 0; i < BUTTONPINCOUNT; i++) {
		int pin = ButtonsIn[i];
		pinMode(pin, INPUT);
		pullUpDnControl(pin, PUD_DOWN);
	}
}

int main(int argc, char ** args) {
	wiringPiSetup();

	int j,k;
	while(1) {
		for(j = 0; j < OEPINCOUNT; j++) {
			clearAllOE();
			digitalWrite(OEOut[j], LOW);
			for(k = 0; k < BUTTONPINCOUNT; k++) {
				int pin = ButtonsIn[k];
				if(k == 0) {
					delayMicroseconds(1);
					//add a delay to prevent BUTTON-ZERO measuring bug
				}
				if(!digitalRead(pin)) {
					printf("Button pressed! (OE:%d, BUTTON:%d)\n",j,k);
				}
			}
		}
	}
}
