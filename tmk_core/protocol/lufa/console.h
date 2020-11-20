#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void console_init(void);
bool console_configure(void);
void console_control_request(void);
bool console_is_ready(void);
void console_task(void);

#ifdef __cplusplus
}
#endif

#endif
