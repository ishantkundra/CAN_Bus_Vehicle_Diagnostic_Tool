#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "can.h"
#include "oled.h"

#define PID_ENGINE_RPM 0x0C
#define PID_VEHICLE_SPEED 0x0D

void parseOBDResponse(uint8_t pid, uint8_t* data, uint8_t length) {
    if (pid == PID_ENGINE_RPM && length >= 4) {
        uint16_t A = data[2];
        uint16_t B = data[3];
        float rpm = ((A * 256) + B) / 4.0;
        char buf[32];
        sprintf(buf, "RPM: %.0f", rpm);
        oled_print_line(0, buf);
    } else if (pid == PID_VEHICLE_SPEED && length >= 3) {
        uint8_t A = data[2];
        char buf[32];
        sprintf(buf, "Speed: %d km/h", A);
        oled_print_line(1, buf);
    }
}

void requestOBDData(uint8_t pid) {
    CAN_Message msg;
    msg.id = 0x7DF;
    msg.length = 8;
    msg.data[0] = 0x02;
    msg.data[1] = 0x01;
    msg.data[2] = pid;
    for (int i = 3; i < 8; i++) msg.data[i] = 0x00;

    can_send(&msg);
}

void main_loop() {
    oled_init();
    can_init();

    while (1) {
        requestOBDData(PID_ENGINE_RPM);
        requestOBDData(PID_VEHICLE_SPEED);
        CAN_Message rx;
        if (can_receive(&rx)) {
            parseOBDResponse(rx.data[2], rx.data, rx.length);
        }
        delay(1000);
    }
}