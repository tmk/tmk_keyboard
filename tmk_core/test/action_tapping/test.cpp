// copied from: https://github.com/ShawnHymel/c-unit-test
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

int main(int ac, char ** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}


extern "C" {
    #include "action_code.h"
    #include "actionmap.h"
    #include "action_tapping.h"
    //#include "keyboard.h"
    //#include "report.h"
    #include "debug.h"
    //#include "print.h"
    #include "host.h"
    #include "wait.h"
    #include "bootloader.h"
}


static uint16_t timer_ms = 0;

void wait_ms(uint32_t ms) { (void)ms;  timer_ms += ms; printf("wait_ms: %d\n", ms); }
uint16_t timer_read(void) { return timer_ms; }
void keyboard_set_leds(uint8_t leds) { (void)leds; }
int print(char* s) { return printf("%s", s); }
int println(char* s) { return printf("%s%s", s, "\r\n"); }
void bootloader_jump(void) {}

void host_keyboard_send(report_keyboard_t *report) {
    (void)report;
    printf("host_keyboard_send: ");
    for (int i = 0; i < 8; i++) { printf("%02X ", report->raw[i]); }
    printf("\n");
}

#define D_MK        ACTION_DUAL_MODS_KEY(MOD_LCTL, KC_B)
/*
extern const action_t _actionmaps[][MATRIX_ROWS][MATRIX_COLS];
action_t action_for_key(uint8_t layer, keypos_t key)
{
    action_t a = _actionmaps[(layer)][(key.row)][(key.col)];
    return a;
}

const action_t _actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // layer 0
    {
        { AC_A,  D_MK,  AC_C,  AC_D }   // row 0
    },
    // layer 1
    {
        { AC_E,  AC_F,  AC_G,  AC_H }   // row 0
    },
};
*/

action_t action_for_key(uint8_t layer, keypos_t key)
{
    return (action_t[][MATRIX_ROWS][MATRIX_COLS]){
        {
            { AC_A,  D_MK,  AC_C,  AC_D }
        },
        {
            { AC_E,  AC_F,  AC_G,  AC_H }
        },
    } [(layer)][(key.row)][(key.col)];
};




// Create a test group
TEST_GROUP(average_test_group)
{
    void setup()
    {
        // Initialize before each test
        //debug_enable = true;
    }

    void teardown()
    {
        // Deinitialize after each test
    }
};

#define REC(_row, _col, _pressed, _time)     { .event = { .key = { .col = _col, .row = _row }, .pressed = _pressed, .time = _time }, .tap = {}}


#define RECORD(_event, _tap)                    { .event = _event, .tap = _tap}
#define EVENT(_key, _pressed, _time)            { .key = _key, .pressed = _pressed, .time = _time }
#define KEY(_row, _col)                         { .col = _col, .row = _row }

#define KEY_PRESS(_row, _col, _time)            RECORD(EVENT(KEY(_row, _col), true, _time), {})
#define KEY_RELEASE(_row, _col, _time)          RECORD(EVENT(KEY(_row, _col), false, _time), {})

// Test the average function
TEST(average_test_group, simple_test)
{
    action_tapping_process(KEY_PRESS(0, 1, 1));
    //action_tapping_process( REC(1, 1, true, 1) );
    //action_tapping_process( RECORD(EVENT(KEY(1, 1), true, 1), {}) );
    //action_tapping_process({ .event = { .key = { .col = 1, .row = 1 }, .pressed = true, .time = 1 }, .tap = {}});
    //
    //wait_ms(10);
    //
    action_tapping_process(KEY_RELEASE(0, 1, 100));
    //action_tapping_process({ .event = { .key = { .col = 1, .row = 1 }, .pressed = false, .time = 100 }, .tap = {}});
    CHECK_EQUAL(1, 1.0);
}

// Test null array
TEST(average_test_group, null_test)
{
    CHECK_EQUAL(0, 0.0);
}
