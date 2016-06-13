#include <RH_ASK.h>
#include "const.h"
#include "lcd.h"

#define DEBUG

#define die()	while (1)

enum pins {
	RECEIVE_PIN = 11,
	LED1_PIN = 13,
};

RH_ASK radio(2000, RECEIVE_PIN, NULL);

static void led1_on(void) {
	digitalWrite(LED1_PIN, HIGH);
}

static void led1_off(void) {
	digitalWrite(LED1_PIN, LOW);
}

static void led1_toggle(void) {
	int x = digitalRead(LED1_PIN);
	digitalWrite(LED1_PIN, !x);
}

void setup() {
#	ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Initializing radio");
#	endif

	pinMode(LED1_PIN, OUTPUT);

	led1_off();

	if (!radio.init()) {
#		ifdef DEBUG
		Serial.println("Init failed");
#		endif
		die();
	}
}

void loop() {
	uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
	uint8_t buflen = sizeof(buf);

	if (radio.recv(buf, &buflen)) {
#		ifdef DEBUG
		radio.printBuffer("Received:", buf, buflen);
#		endif
		if (strncmp((const char *) buf, msg1, buflen)) {
			led1_on();
			delay(500);
			led1_off();
		} else if (strncmp((const char *) buf, msg2, buflen)) {
			led1_on();
			delay(500);
			led1_off();
		}
	}
}
