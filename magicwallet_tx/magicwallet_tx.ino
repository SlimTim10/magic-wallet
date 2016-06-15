#include <RH_ASK.h>

/* #define DEBUG */

#define die()	while (1)

enum pins {
	TRANSMIT_PIN = 12,
	LED1_PIN = 13,
	BUTTON1_PIN = 9,
	BUTTON2_PIN = 8,
};

RH_ASK radio(1000, NULL, TRANSMIT_PIN);

static void led1_on(void) {
	digitalWrite(LED1_PIN, HIGH);
}

static void led1_off(void) {
	digitalWrite(LED1_PIN, LOW);
}

static void led1_toggle(void) {
	uint8_t x = digitalRead(LED1_PIN);
	digitalWrite(LED1_PIN, !x);
}

static uint8_t button1_pressed(void) {
	return (digitalRead(BUTTON1_PIN) == LOW);
}

static uint8_t button2_pressed(void) {
	return (digitalRead(BUTTON2_PIN) == LOW);
}

const char *str_bill_5 = "5";
const char *str_bill_10 = "10";

void setup() {
#	ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Initializing radio");
#	endif

	pinMode(LED1_PIN, OUTPUT);
	pinMode(BUTTON1_PIN, INPUT_PULLUP);
	pinMode(BUTTON2_PIN, INPUT_PULLUP);

	led1_off();

	if (!radio.init()) {
#		ifdef DEBUG
		Serial.println("Init failed");
#		endif
		led1_on();
		die();
	}
}

void loop() {
	if (button1_pressed()) {
		for (uint8_t i = 0; i < 3; i++) {
			radio.send((uint8_t *) str_bill_5, strlen(str_bill_5));
			radio.waitPacketSent();
		}
		led1_on();
		delay(200);
		led1_off();
	} else if (button2_pressed()) {
		for (uint8_t i = 0; i < 3; i++) {
			radio.send((uint8_t *) str_bill_10, strlen(str_bill_10));
			radio.waitPacketSent();
		}
		led1_on();
		delay(200);
		led1_off();
	}
}
