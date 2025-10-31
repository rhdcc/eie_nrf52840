/*
 * sm.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#include <zephyr/smf.h> // unused

typedef struct {
    struct smf_ctx ctx;
    uint8_t ascii_val;
    uint32_t bit_count;
    uint16_t ms_count;
} global_state_object_t;

static global_state_object_t g_state;

// forward declarations
static void s1_entry(void *o);
static enum smf_state_result s1_run(void *o);
static void s1_exit(void *o);

struct smf_state s1_state = SMF_CREATE_STATE(s1_entry, s1_run, s1_exit, NULL, NULL);

void sm_init(void) {
    smf_set_initial(SMF_CTX(&g_state), &s1_state);
}

void sm_run(void) {
    smf_run_state(SMF_CTX(&g_state));
}

static void s1_entry(void *o) {
}

static enum smf_state_result s1_run(void *o) {
    if(g_state.ms_count >= 1000) {
        g_state.ms_count = 0;
        LED_toggle(LED3);
    }
    if(BTN_check_clear_pressed(BTN0)) {
        // blink led0
        g_state.ascii_val = g_state.ascii_val | (0x1 << g_state.bit_count++);
    } else if(BTN_check_clear_pressed(BTN1)) { // TODO: else if?
        // blink led1
        g_state.bit_count += 1;
    }

    if(g_state.bit_count >= 8) {
        printk("Reached 8 bits, resetting ascii value\n");
        g_state.bit_count = 0;
    }

    g_state.ms_count += 1; // NOTE: Determined by MS_SLEEP in main.c
    return SMF_EVENT_HANDLED;
}

static void s1_exit(void *o) {
}



