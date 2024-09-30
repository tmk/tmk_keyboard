#include "timer.h"
#include "wait.h"
#include "host.h"
#include "report.h"
#include "mousekey.h"
#include "mouse.h"


static report_mouse_t report = {};

uint8_t mouse_buttons(void)
{
    return report.buttons;
}

void mouse_send(report_mouse_t *r)
{
    report = *r;
#ifdef MOUSEKEY_ENABLE
    // buttons integration between mouse and mousekey
    report.buttons |= mousekey_buttons();
#endif
    host_mouse_send(&report);
}
