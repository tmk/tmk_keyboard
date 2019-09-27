#include <stdbool.h>
#include "debug.h"

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

debug_config_t debug_config = {
/* GCC Bug 10676 - Using unnamed fields in initializers
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=10676 */
#if GCC_VERSION >= 40600
    .enable = false,
    .matrix = false,
    .keyboard = false,
    .mouse = false,
    .reserved = 0
#else
    {
        false,  // .enable
        false,  // .matrix
        false,  // .keyboard
        false,  // .mouse
        0       // .reserved
    }
#endif
};

bool debug_inline = false, debug_empty_matrix = false, debug_empty_report = false;

#ifndef NO_PRINT
void print_scancode (uint8_t code, uint8_t error, char prefix)
{
  if (!debug_keyboard) return;
  if (!debug_inline) print("keyboard>");
  debug_inline= true;
  xputc(' ');
  if (prefix) xputc(prefix);
  xprintf("%02X", code);
  if (error) xprintf("!%X", error);
}

void print_modcode (uint8_t code, char prefix)
{
  if (!debug_keyboard) return;
  if (!debug_inline) print("keyboard>");
  debug_inline= true;
  xputc(' ');
  if (prefix) xputc(prefix);
  xprintf("M%X", code);
}
#endif
