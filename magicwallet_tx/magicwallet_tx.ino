#include <RH_ASK.h>

#define DEBUG

#define die()	while (1)

enum pins {
	TRANSMIT_PIN = 12,
	BUTTON1_PIN = 6,
	BUTTON2_PIN = 7,
	BUTTON3_PIN = 8,
	BUTTON4_PIN = 9,
};

RH_ASK radio(1000, NULL, TRANSMIT_PIN);

static uint8_t button1_pressed(void) {
	return (digitalRead(BUTTON1_PIN) == LOW);
}

static uint8_t button2_pressed(void) {
	return (digitalRead(BUTTON2_PIN) == LOW);
}

static uint8_t button3_pressed(void) {
	return (digitalRead(BUTTON3_PIN) == LOW);
}

static uint8_t button4_pressed(void) {
	return (digitalRead(BUTTON4_PIN) == LOW);
}

struct bill {
	uint8_t done;
	const char *msg;
};

struct {
	bill b20;
	bill b5;
	bill b10;
	bill b50;
} bills = {
	{0, "20"},
	{0, "5"},
	{0, "10"},
	{0, "50"},
};

void setup() {
#	ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Initializing radio");
#	endif

	pinMode(BUTTON1_PIN, INPUT_PULLUP);
	pinMode(BUTTON2_PIN, INPUT_PULLUP);
	pinMode(BUTTON3_PIN, INPUT_PULLUP);
	pinMode(BUTTON4_PIN, INPUT_PULLUP);

	if (!radio.init()) {
#		ifdef DEBUG
		Serial.println("Init failed");
#		endif
		die();
	}
}

void loop() {
	if (!bills.b20.done && button1_pressed()) {
		for (uint8_t i = 0; i < 3; i++) {
			radio.send((uint8_t *) bills.b20.msg, strlen(bills.b20.msg));
			radio.waitPacketSent();
			delay(50);
#			ifdef DEBUG
			Serial.print("Sent: ");
			Serial.println(bills.b20.msg);
#			endif
		}
		bills.b20.done = 1;
	} else if (!bills.b5.done && button2_pressed()) {
		for (uint8_t i = 0; i < 3; i++) {
			radio.send((uint8_t *) bills.b5.msg, strlen(bills.b5.msg));
			radio.waitPacketSent();
			delay(50);
#			ifdef DEBUG
			Serial.print("Sent: ");
			Serial.println(bills.b5.msg);
#			endif
		}
		bills.b5.done = 1;
	} else if (!bills.b10.done && button3_pressed()) {
		for (uint8_t i = 0; i < 3; i++) {
			radio.send((uint8_t *) bills.b10.msg, strlen(bills.b10.msg));
			radio.waitPacketSent();
			delay(50);
#			ifdef DEBUG
			Serial.print("Sent: ");
			Serial.println(bills.b10.msg);
#			endif
		}
		bills.b10.done = 1;
	} else if (!bills.b50.done && button4_pressed()) {
		for (uint8_t i = 0; i < 3; i++) {
			radio.send((uint8_t *) bills.b50.msg, strlen(bills.b50.msg));
			radio.waitPacketSent();
			delay(50);
#			ifdef DEBUG
			Serial.print("Sent: ");
			Serial.println(bills.b50.msg);
#			endif
		}
		bills.b50.done = 1;
	}
}
