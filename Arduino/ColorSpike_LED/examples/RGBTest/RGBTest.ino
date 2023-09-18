#include <ColorSpike_LED.h>

#include <stdint.h>

// Pin assignments

#define DCLK 0
#define DI 2
#define LED LED_BUILTIN

ColorSpike_LED cs(DCLK, DI);

void setup() {
  cs.init();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  cs.clear();
  cs.setR(3,0x3f);
  digitalWrite(LED, HIGH);
  cs.update();
}


uint16_t cycle = 0;

void loop() {
  cs.clear();
  cs.setR(cycle%60,0x1f);
  cs.setG((int)(cycle*1.3)%60,0x1f);
  cs.setB((int)(cycle*1.1)%60,0x1f);
  cs.update();
  cycle++;
}
