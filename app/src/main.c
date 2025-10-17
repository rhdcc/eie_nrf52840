/*
 * main.c
 */

#include <zephyr/kernel.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

typedef enum lock_state_t {
    LOCKED,
    WAITING
} lock_state;

#define MAX_PASSOWRD_LEN 10

typedef struct password_t {
    int count;
    int chars[MAX_PASSOWRD_LEN];
} password;

bool pwsd_add_char(password *p, int c) {
    if(p->count >= MAX_PASSOWRD_LEN) return false;
    p->chars[p->count++] = c;
    return true;
}

int main(void) {
    if(0 > BTN_init()) {
        return 0;
    }
    if(0 > LED_init()) {
        return 0;
    }

    password good = {
        .chars = {0, 1, 2, 0, 1, 2, -1, -1, -1, -1},
        .count = 6
    };
    password users = {0};

    lock_state state = LOCKED;
    bool flush = false;
    while(1) {
        LED_set(LED0, 1-(int)state);
        k_msleep(SLEEP_MS);
        int buttons[4] = {0};
        for(int i = 0; i < 4; ++i) {
            if(BTN_check_clear_pressed(i)) {
                buttons[i] = 1;
            }
        }

        if(state == WAITING) {
            for(int i = 0; i < 4; ++i) {
                if(buttons[i] != 0) {
                    state = LOCKED;
                    break;
                }
            }
            continue;
        }

        if(buttons[BTN0]) {
            flush = !pwsd_add_char(&users, 0);
        }
        if(buttons[BTN1]) {
            flush = !pwsd_add_char(&users, 1);
        }
        if(buttons[BTN2]) {
            flush = !pwsd_add_char(&users, 2);
        }

        if(flush) {
            printk(" NOTE: Max password limit exceeded\n");
        }

        if(buttons[BTN3]) {
            flush = true;
        }
        if(flush) {
            bool correct = true;
            if(users.count != good.count) correct = false;
            for(int i = 0; i < users.count; ++i) {
                if(users.chars[i] != good.chars[i]) {
                    correct = false;
                }
            }
            if(correct) {
                printk("Correct!\n");
            } else {
                printk("Incorrect!\n");
            }
            users.count = 0;
            flush = false;
            state = WAITING;
        }
    }
    return 0;
}
