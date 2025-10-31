/*
 * main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#include "sm.h"

#define SLEEP_MS 1

int main(void) {
    if (0 > BTN_init()) {
        return 0;
    }
    if (0 > LED_init()) {
        return 0;
    }

    sm_init();

    while(1) {
        sm_run();
        k_msleep(SLEEP_MS);
    }
    return 0;
}
