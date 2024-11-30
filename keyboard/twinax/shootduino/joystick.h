#ifndef __JOYSTICK_H_
#define __JOYSTICK_H_
#include <avr/pgmspace.h>

struct JoystickState {
  bool left : 1;
  bool right : 1;
  bool up : 1;
  bool down : 1;
  bool joy_button : 1;
#ifdef JOYSTICK_SHIELD
  bool top_button : 1;
  bool bottom_button : 1;
#endif
  bool left_button : 1;
  bool right_button : 1;
};

extern struct JoystickState joystick;

#ifdef __cplusplus
  extern "C" {
#endif
void update_joystick(void);
void init_joystick(void);
#ifdef __cplusplus
  }
#endif

#endif

