
#include "SoftwareSerial.h"

#ifndef D5
#if defined(ESP8266)
#define D5 (14)
#define D6 (12)
#elif defined(ESP32)
#define TX1 (32)
#define RX1 (33)
#define TX2 (34)
#define RX2 (35)
#endif
#endif

SoftwareSerial swSer1;
SoftwareSerial swSer2;

void setup() {
	delay(2000);
	Serial.begin(9600);
	Serial.println(PSTR("\nOne Wire Half Duplex Serial Tester"));
	swSer1.begin(9600, SWSERIAL_8N1, RX1, TX1, false);
	// high speed half duplex, turn off interrupts during tx
	swSer1.enableIntTx(false);
	swSer2.begin(9600, SWSERIAL_8N1, RX2, TX2, false);
	// high speed half duplex, turn off interrupts during tx
	swSer2.enableIntTx(false);
}

void checkSwSerial(SoftwareSerial*);

void loop() {
	Serial.println(PSTR("\n\nTesting on swSer1"));
	Serial.print(PSTR("Enter something to send using swSer1."));
	checkSwSerial(&swSer1);

	Serial.println(PSTR("\n\nTesting on swSer2"));
	Serial.print(PSTR("Enter something to send using swSer2."));
	checkSwSerial(&swSer2);
}

void checkSwSerial(SoftwareSerial* ss) {
	byte ch;
	while (!Serial.available());
	ss->enableTx(true);
	while (Serial.available()) {
		ch = Serial.read();
		ss->write(ch);
	}
	ss->enableTx(false);
	// wait 1 second for the reply from SOftwareSerial if any
	delay(1000);
	if (ss->available()) {
		Serial.print(PSTR("\nResult:"));
		while (ss->available()) {
			ch = (byte)ss->read();
			Serial.print(ch < 0x10 ? PSTR(" 0") : PSTR(" "));
			Serial.print(ch, HEX);
		}
		Serial.println();
	}
}