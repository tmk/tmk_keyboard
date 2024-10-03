#ifndef MOUSE_H
#define  MOUSE_H

#include "stdint.h"
#include "report.h"

enum { MOUSE_SCROLL_V, MOUSE_SCROLL_H, MOUSE_SCROLL_VH };

#ifdef __cplusplus
extern "C" {
#endif

uint8_t mouse_buttons(void);

void mouse_send(report_mouse_t *report);

#ifdef __cplusplus
}
#endif

#endif
