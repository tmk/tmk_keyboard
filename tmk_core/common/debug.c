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
    .scancode = false,
    .reserved = 0
#else
    {
        false,  // .enable
        false,  // .matrix
        false,  // .keyboard
        false,  // .mouse
        false,  // .scancode
        0       // .reserved
    }
#endif
};

bool debug_inline = false;

#ifndef NO_PRINT
void print_scancode (uint8_t code, uint8_t error, char prefix)
{
  if (!debug_scancode) return;
  if (!debug_inline) print("Key:");
  debug_inline= true;
  if (prefix) xprintf(" %c%02X", prefix, code);
  else        xprintf(  " %02X",         code);
  if (error)  xprintf("!%X", error);
}

void print_modcode (uint8_t code, char prefix)
{
  if (!debug_scancode) return;
  if (!debug_inline) print("Key:");
  debug_inline= true;
  if (prefix) xprintf(" %cM%X", prefix, code);
  else        xprintf(  " M%X",         code);
}
#endif
