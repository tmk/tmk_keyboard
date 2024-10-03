#include "timer.h"
#include "wait.h"
#include "host.h"
#include "report.h"
#include "mousekey.h"
#include "mouse.h"


/*
 * configurable parmeters
 */
// use MOUSE_BTN* defined in report.h
#if !defined(MOUSE_SCROLL_BUTTON)
#   define MOUSE_SCROLL_BUTTON 0
#endif
uint8_t mouse_scroll_button = MOUSE_SCROLL_BUTTON;

#if !defined(MOUSE_SCROLL_MODE)
#   define MOUSE_SCROLL_MODE MOUSE_SCROLL_V
#endif
uint8_t mouse_scroll_mode = MOUSE_SCROLL_MODE;

#if !defined(MOUSE_SCROLL_BUTTON_TIMEOUT)
#   define MOUSE_SCROLL_BUTTON_TIMEOUT 200
#endif
uint8_t mouse_scroll_button_timeout = MOUSE_SCROLL_BUTTON_TIMEOUT;

#if !defined(MOUSE_SCROLL_DIV)
#   define MOUSE_SCROLL_DIV 10
#endif
uint8_t mouse_scroll_div = MOUSE_SCROLL_DIV;



static report_mouse_t report = {};

uint8_t mouse_buttons(void)
{
    return report.buttons;
}

void mouse_send(report_mouse_t *r)
{
    report = *r;

    //TODO: other features; middle button/third button emuation(evdev/libinput)

#if MOUSE_SCROLL_BUTTON
    static enum { STATE_NONE, STATE_BTN, STATE_SENT } scroll_state = STATE_NONE;
    static uint16_t scroll_btn_time = 0;
    static int8_t x_remain = 0;
    static int8_t y_remain = 0;
    if (report.buttons & mouse_scroll_button) {
        report.buttons &= ~mouse_scroll_button;

        if (scroll_state == STATE_NONE) {
            scroll_btn_time = timer_read();
            scroll_state = STATE_BTN;
        }

        report.x += x_remain;
        report.y += y_remain;
        switch (mouse_scroll_mode) {
        case MOUSE_SCROLL_V:
            report.v = -(report.y / mouse_scroll_div);
            y_remain = report.y % mouse_scroll_div;
            report.h = 0;
            break;
        case MOUSE_SCROLL_H:
            report.h = -(report.x / mouse_scroll_div);
            x_remain = report.x % mouse_scroll_div;
            report.v = 0;
            break;
        case MOUSE_SCROLL_VH:
            report.v = -(report.y / mouse_scroll_div);
            y_remain = report.y % mouse_scroll_div;
            report.h = -(report.x / mouse_scroll_div);
            x_remain = report.x % mouse_scroll_div;
            break;
        }

        report.x = 0;
        report.y = 0;
        if (report.v || report.h) {
            scroll_state = STATE_SENT;
        }
    } else {
        if (scroll_state == STATE_BTN) {
            if (timer_elapsed(scroll_btn_time) < mouse_scroll_button_timeout) {
                // send scroll button
                report.buttons |= mouse_scroll_button;
                report.x = 0;
                report.y = 0;
                report.v = 0;
                report.h = 0;
                host_mouse_send(&report);
                wait_ms(100);
                report.buttons &= ~mouse_scroll_button;
            }
        }

        scroll_state = STATE_NONE;
        scroll_btn_time = 0;
    }
#endif

#ifdef MOUSEKEY_ENABLE
    // buttons integration between mouse and mousekey
    report.buttons |= mousekey_buttons();
#endif
    host_mouse_send(&report);
}
