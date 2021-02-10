#ifndef IBMPC_USB_H
#define IBMPC_USB_H

typedef enum { NONE, PC_XT, PC_AT, PC_TERMINAL } keyboard_kind_t;
#define KEYBOARD_KIND_STR(kind) \
    (kind == PC_XT ? "XT" :   \
     kind == PC_AT ? "AT" :   \
     kind == PC_TERMINAL ? "TERMINAL" :   \
     "NONE")


extern uint16_t keyboard_id;
extern keyboard_kind_t keyboard_kind;

#endif
