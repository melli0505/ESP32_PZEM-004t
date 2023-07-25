#include <Arduino.h>
#include <PZEM004Tv30.h>

#include "config.h"

#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

#define PZEM_SERIAL Serial2
#define CONSOLE_SERIAL Serial

static PZEM004Tv30* pzem;

void setup_pzem(PZEM004Tv30* pzem_instance) {
    pzem = pzem_instance;
    Serial.print("Custom Address:");
    Serial.println(pzem->readAddress(), HEX);
}

float get_voltage() {
    float voltage = pzem->voltage();
    if (isnan(voltage)) {
        return 0;
    } else {
        return voltage;
    }
}

float get_current() {
    float current = pzem->current();
    if (isnan(current)) {
        return 0;
    } else {
        return current;
    }
}

float get_power() {
    float power = pzem->power();
    if (isnan(power)) {
        return 0;
    } else {
        return power;
    }
}

float get_energy() {
    float energy = pzem->energy();
    if (isnan(energy)) {
        return 0;
    } else {
        return energy;
    }
}

float get_frequency() {
    float frequency = pzem->frequency();
    if (isnan(frequency)) {
        return 0;
    } else {
        return frequency;
    }
}

float get_pf() {
    float pf = pzem->pf();
    if (isnan(pf)) {
        return 0;
    } else {
        return pf;
    }
}

bool reset_energy_counter() {
    bool isReset = pzem->resetEnergy();
    return isReset;
}