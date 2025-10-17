/*
 * main.c
 */

#include <zephyr/kernel.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

int main(void) {
    if(0 > BTN_init()) {
        return 0;
    }
    if(0 > LED_init()) {
        return 0;
    }

    int counter = 0;
    while(1) {
        if(BTN_check_clear_pressed(BTN0)) {
            LED_set(LED0, (counter >> 0) & 0x1);
            LED_set(LED1, (counter >> 1) & 0x1);
            LED_set(LED2, (counter >> 2) & 0x1);
            LED_set(LED3, (counter >> 3) & 0x1);
            printk("Button 0 pressed!\n");
            counter = (counter + 1) % 16;
        }
        k_msleep(SLEEP_MS);
    }
    return 0;
}
