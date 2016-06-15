#include <RH_ASK.h>
#include "lcd.h"

/* #define DEBUG */

#define die()	while (1)

enum pins {
	RECEIVE_PIN = 11,
};

enum { MAX_BILLS = 4 };

RH_ASK radio(1000, RECEIVE_PIN, NULL);
uint8_t bills[MAX_BILLS];
uint8_t bills_count;
uint8_t prev_bill;

void setup() {
#	ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Initializing radio");
#	endif

	lcd_init();
	lcd_cursor_off();
	lcd_clear();
	lcd_printf("Ready");

	for (uint8_t i = 0; i < MAX_BILLS; i++) {
		bills[i] = 0;
	}
	bills_count = 0;
	prev_bill = 0;

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
		buf[buflen] = '\0';
		uint8_t n = atoi((const char *) buf);
		if (n != prev_bill) {
			bills[bills_count] = n;
			lcd_clear();
			lcd_printf("%2d%2d%2d%2d", bills[0], bills[1], bills[2], bills[3]);
			bills_count = (bills_count + 1) % MAX_BILLS;
			prev_bill = n;
		}
#		ifdef DEBUG
		radio.printBuffer("Received:", buf, buflen);
		Serial.println("bills:");
		Serial.println(bills[0]);
		Serial.println(bills[1]);
		Serial.println(bills[2]);
		Serial.println(bills[3]);
		Serial.println();
#		endif
	}
}
