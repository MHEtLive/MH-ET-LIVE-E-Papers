#include <SPI.h>
#include "epdif.h"

EpdIf::EpdIf() {};

EpdIf::~EpdIf() {};

void EpdIf::Transfer(unsigned char data) {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(data);
    digitalWrite(CS_PIN, HIGH);
}

unsigned char EpdIf::IfInit(void) {
    pinMode(CS_PIN  , OUTPUT);
    pinMode(RST_PIN , OUTPUT);
    pinMode(DC_PIN  , OUTPUT);
    pinMode(BUSY_PIN, INPUT); 
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.begin();
    return 0;
}

