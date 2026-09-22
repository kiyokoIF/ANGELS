#include <Arduino.h>
#include <ESP_I2S.h>

#define I2S_FS   3
#define I2S_SCK  2
#define I2S_DIN  10
#define I2S_MCK  3

#define BUFFER_SIZE 64 // Must be a multiple of 2 as 16 bit data size.

I2SClass *i2s;

char buf[BUFFER_SIZE];

int refH = 32767;
int refL = -32768;

void InitI2S() {
  

  Serial.println("Initializing i2s");

  i2s->setPins(I2S_SCK, I2S_FS, -1, I2S_DIN);
  i2s->setInverted(true, false); // seed outputs falling edge sync clock so inverted
  if (!i2s->begin(I2S_MODE_STD, 8000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO, -1, I2S_ROLE_SLAVE)) {
    Serial.println("Failed to initialize I2S bus!");
    while (1);
  }
}

void setup() {
  Serial.begin(2000000);
  pinMode(8, OUTPUT);

  i2s = new I2SClass(I2S_NUM_0);

  InitI2S();
}

void loop() {
  // 16-bit MSB left-justified
  if (i2s->readBytes(buf, BUFFER_SIZE) != BUFFER_SIZE) {
    Serial.println("FAIL");
    i2s->end();
    InitI2S();
    
    delay(1000);
    Serial.println("Restarting in 3...");
    delay(1000);
    Serial.println("Restarting in 2...");
    delay(1000);
    Serial.println("Restarting in 1...");
    delay(1000);
  } 

  int16_t x = 0;
  x |= buf[0] << 8;
  x |= buf[1];
  Serial.printf("i2sIn:%d,l:%d,h:%d\n", x, refH, refL);
}