#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "split_util.h"
#include "config.h"
#include "transactions.h"

#define ERROR_DISCONNECT_COUNT 1
#define ROWS_PER_HAND (MATRIX_ROWS / 2)

static const pin_t row_pins[ROWS_PER_HAND] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS]   = MATRIX_COL_PINS;

/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
extern matrix_row_t matrix[MATRIX_ROWS];      // debounced values

// row offsets for each hand
uint8_t thisHand, thatHand;

// user-defined overridable functions
__attribute__((weak)) void matrix_slave_scan_kb(void) { matrix_slave_scan_user(); }
__attribute__((weak)) void matrix_slave_scan_user(void) {}

static void init_pins(void) {
    for (size_t i = 0; i < MATRIX_COLS; i++) {
        setPinInputHigh(col_pins[i]);
    }
    for (size_t i = 0; i < ROWS_PER_HAND; i++) {
/* We deliberatly choose "slower" push pull pins,
 * those are fast enough but with lower driving currents
 * should produce less EMI noise on the lines. */
#if defined(__riscv)
        /* This is actually implemented as a 2MHZ PP output. */
        palSetLineMode(row_pins[i], PAL_MODE_UNCONNECTED);
#else
        palSetLineMode(row_pins[i], (PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_LOWEST));
#endif
        writePinHigh(row_pins[i]);
    }
}

void matrix_init(void) {
    split_pre_init();

    thisHand = isLeftHand ? 0 : (ROWS_PER_HAND);
    thatHand = ROWS_PER_HAND - thisHand;

    // initialize key pins
    init_pins();

    // initialize matrix state: all keys off
    memset(raw_matrix, 0, sizeof(raw_matrix));
    memset(matrix, 0, sizeof(matrix));

    debounce_init(ROWS_PER_HAND);

    matrix_init_quantum();

    split_post_init();
    dprintln("end init");
}

static inline bool matrix_post_scan(void) {
    register bool changed = false;
            dprintln("Enter post matrix_scan");
    if (is_keyboard_master()) {
        static bool         error;
        static bool         peripheral_connected = true;
        static fast_timer_t last_connection;
        matrix_row_t        slave_matrix[ROWS_PER_HAND];

        if (peripheral_connected) {
            if (!transport_master(matrix + thisHand, slave_matrix)) {
                error = true;
                // reset other half if disconnected
                memset(&matrix[thatHand], 0, sizeof(slave_matrix));
                peripheral_connected = false;
                changed              = true;
                dprintln("Peripheral disconnected. Attempt to re-connect in 5 seconds.");
            } else {
                if (error) {
                    error                = false;
                    peripheral_connected = true;
                    last_connection      = timer_read_fast();
                    dprintln("Peripheral connected.");
                }

                if (memcmp(&matrix[thatHand], slave_matrix, sizeof(slave_matrix)) != 0) {
                    memcpy(&matrix[thatHand], slave_matrix, sizeof(slave_matrix));
                    changed = true;
                }
            }
        } else if (timer_elapsed32(last_connection) > 5000) {
            dprintln("Peripheral connection attempt.");
            peripheral_connected = true;
            last_connection      = timer_read_fast();
        }

        matrix_scan_quantum();
    } else {
        transport_slave(matrix + thatHand, matrix + thisHand);

        matrix_slave_scan_kb();
    }

    return changed;
}

uint8_t matrix_scan(void) {
    bool         local_changed = false;
    matrix_row_t current_matrix[ROWS_PER_HAND];

    for (size_t row_idx = 0; row_idx < ROWS_PER_HAND; row_idx++) {
        /* Drive row pin low. */
        writePinLow(row_pins[row_idx]);
        while (readPin(row_pins[row_idx]) != 0)
            ;

        uint16_t porta = palReadPort(GPIOA);
        uint16_t portb = palReadPort(GPIOB);

        printf("Porta: %d \n", porta);
        printf("Portb: %d \n", porta);


        /* Drive row pin high again. */
        writePinHigh(row_pins[row_idx]);

        /* Order of pins is: B15, B14, B13, B2, B1, B0, A7, A2
           Pin is active low, therefore we have to invert the result. */
        /*matrix_row_t cols = ~(((porta & 0x4) >> 2) | ((porta & 0x80) >> 6) | ((portb & 0x7) << 2) | ((portb & 0xE000) >> 8));*/
        
        /* Order of pins is: A2, B13, A8, A6, B15, A7, B14, B0
           Pin is active low, therefore we have to invert the result. */
        matrix_row_t cols = ~(((portb & 0x1)) | ((portb & 0x4000) >> 13) | ((porta & 0x80) >> 5) | ((portb & 0xE8000) >> 12) | ((porta & 0x40) >> 1) | ((porta & 0x100) >> 2) | ((portb & 0x2000) >> 7) | ((porta & 0x4) << 5));

        /* Reverse the order of columns for left hand as the board is flipped. */
        if (isLeftHand) {
#if defined(__arm__)
            /* rbit assembly reverses bit order of 32bit registers. */
            uint32_t temp = cols;
            __asm__("rbit %0, %1" : "=r"(temp) : "r"(temp));
            cols = temp >> 24;
#else
            /* RISC-V bit manipulation extension not present. Use bit-hack.
            https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith32Bits */
            cols = (matrix_row_t)(((cols * 0x0802LU & 0x22110LU) | (cols * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);
#endif
        }

        current_matrix[row_idx] = cols;

        /* Wait until col pins are high again. */
        size_t counter = 0xFF;
        while (((palReadGroup(GPIOA, 0x84, 0) != 0x84) || ((palReadGroup(GPIOB, 0xE007, 0) != 0xE007))) && counter != 0) {
            dprintln("Waiting");
            counter--;
        }
    }

    if (memcmp(raw_matrix, current_matrix, sizeof(current_matrix)) != 0) {
        memcpy(raw_matrix, current_matrix, sizeof(current_matrix));
        local_changed = true;
    }

    debounce(raw_matrix, matrix + thisHand, ROWS_PER_HAND, local_changed);

    bool remote_changed = matrix_post_scan();
    return (uint8_t)(local_changed || remote_changed);
}