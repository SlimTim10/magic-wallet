#include <RH_ASK.h>

#define DEBUG

#define die()	while (1)

enum pins {
	TRANSMIT_PIN = 12,
};

RH_ASK radio(1000, NULL, TRANSMIT_PIN);

static uint8_t button_pressed(uint8_t pin) {
	return (digitalRead(pin) == LOW);
}

enum { MAX_BILLS = 4 };

struct bill {
	uint8_t pin;
	uint8_t done;
	const char *msg;
} bills[] = {
	{6, 0, "20"},
	{7, 0, "5"},
	{8, 0, "10"},
	{9, 0, "50"},
};

void setup() {
#	ifdef DEBUG
	Serial.begin(9600);
	Serial.println("Initializing radio");
#	endif

	for (uint8_t i = 0; i < MAX_BILLS; i++) {
		pinMode(bills[i].pin, INPUT_PULLUP);
	}

	if (!radio.init()) {
#		ifdef DEBUG
		Serial.println("Init failed");
#		endif
		die();
	}
}

void loop() {
	for (uint8_t i = 0; i < MAX_BILLS; i++) {
		if (!bills[i].done && button_pressed(bills[i].pin)) {
			for (uint8_t j = 0; j < 3; j++) {
				radio.send((uint8_t *) bills[i].msg, strlen(bills[i].msg));
				radio.waitPacketSent();
				delay(50);
#			ifdef DEBUG
				Serial.print("Sent: ");
				Serial.println(bills[i].msg);
#			endif
			}
			bills[i].done = 1;
		}
	}
}
