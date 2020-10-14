/* This file was copied from MMcM's HP HIL converter.
 * Original is matrix.c at commit 3acefd4 of
 * https://github.com/MMcM/qmk_firmware/tree/mmcm/keyboards/converter/hp_hil
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "debug.h"
#include "matrix.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "timer.h"
#include "led.h"

static matrix_row_t matrix[MATRIX_ROWS];

/*
__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}
*/

#define QUEUE_SIZE 16
static uint16_t frame_queue[QUEUE_SIZE];
static uint8_t frame_queue_in, frame_queue_out;

static inline void queue_clear(void) {
  frame_queue_in = frame_queue_out = 0;
}

static inline bool queue_is_empty(void) {
  return (frame_queue_in == frame_queue_out);
}

static inline bool queue_is_full(void) {
  // One entry wasted to be able to check this easily.
  return (((frame_queue_in + 1) % QUEUE_SIZE) == frame_queue_out);
}

static inline uint16_t queue_remove(void) {
  uint16_t frame = frame_queue[frame_queue_out];
  frame_queue_out = (frame_queue_out + 1) % QUEUE_SIZE;
  return frame;
}

static inline void queue_add(uint16_t frame) {
  frame_queue[frame_queue_in] = frame;
  frame_queue_in = (frame_queue_in + 1) % QUEUE_SIZE;
}

#define SI_PORT PORTD
#define SI_PIN PIND
#define SI_DDR DDRD
#define SI_MASK (1 << 0)

#define SO_PORT PORTD
#define SO_DDR DDRD
#define SO_MASK (1 << 1)

#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED_MASK (1 << 7)

ISR(INT0_vect) {
  // ISR latency: {1,2,3}(executing op) + 5(store PC)+ 3(jmp)
  // prologue: push(2)*13,in,clr = 28clk

  // ldi, ldi: 2clk
  uint16_t frame = 0;
  // ldi, ldi: 2clk
  uint16_t mask = 1 << 14;

  // internal counter: ldi, ldi: 2clk

  // 5us = 80clk@16MHz, 80-10-28-2-2-2
  __builtin_avr_delay_cycles(36); // To center of first bit

  while (true) {
    // sbis 2clk(if)/1clk(else)
    if ((SI_PIN & SI_MASK) != 0) {
      // or, or, rjmp: 4clk
      frame |= mask;
    } else {
      // rjmp: 2clk
      __builtin_avr_delay_cycles(3);
    }

    // lsr, ror: 2clk
    mask >>= 1;

    // subi, sbc, breq: 4clk(true)/3clk(false)
    if (mask == 0) {
      break;
    }

    // 10us = 160clk@16MHz, 160-6-2-3-2
    __builtin_avr_delay_cycles(147);

    // (while) rjmp: 2clk
  }

  if (!queue_is_full()) {
    queue_add(frame);
  }

  EIFR = (1 << INTF0);
}

void frame_send_15(uint16_t frame) {
#if 0
  dprintf("Sending frame: %04X\n", frame);
#endif
  uint16_t mask = 1 << 14;
  cli();
  while (mask != 0) {
    // movw, and, and, or: 4
    // breq 1(if)/2(else)
    if ((frame & mask) != 0) {
      // sbi: 2
      SO_PORT |= SO_MASK;
      // rjmp: 2
    } else {
      // cbi: 2
      SO_PORT &= ~SO_MASK;
      __builtin_avr_delay_cycles(1);
    }
    // 10us/loop = 160clk@16MHz
    // 160-(4+5+2+2+2) = 145
    __builtin_avr_delay_cycles(145);
    // lsr, ror: 2
    mask >>= 1;
    // subi, sbc: 2
    // brne 1(false)/2(true,loop)
  }
  sei();
  _delay_us(4);
}

void frame_send(uint8_t addr, bool is_command, uint8_t data_or_op) {
  uint16_t frame = 1;           // Stop bit; odd parity so initially zero.
  frame |= ((uint16_t)data_or_op << 2);
  if (is_command) {
    frame |= ((uint16_t)1 << 10);
  }
  frame |= ((uint16_t)(addr & 0x7) << 11);
  for (uint8_t i = 2; i < 14; i++) {
    if ((frame & (1 << i)) != 0) {
      frame ^= 2;
    }
  }
  if (!is_command || (data_or_op != 0x10))
      dprintf("%d%s%02X ", addr, (is_command ? "C" : "D"), data_or_op);
  frame_send_15(frame);
}

void frame_send_command(uint8_t addr, uint8_t op) {
  frame_send(addr, true, op);
}

void frame_send_command_all(uint8_t op) {
  frame_send_command(0, op);
}

#define IFC_CMD 0x00
#define EPT_CMD 0x01
#define ELB_CMD 0x02
#define IDD_CMD 0x03
#define DSR_CMD 0x04
#define PST_CMD 0x05
#define RRG_CMD 0x06
#define WRG_CMD 0x07
#define ACF_CMD 0x08
#define POL_CMD 0x10
#define RPL_CMD 0x20
#define RNM_CMD 0x30
#define RNM_CMD 0x30
#define RST_CMD 0x31
#define EXD_CMD 0x32
#define RSC_CMD 0x33
#define DKA_CMD 0x3D
#define EK1_CMD 0x3E
#define EK2_CMD 0x3F
#define PR1_CMD 0x40
#define PRM_CMD 0x47
#define AK1_CMD 0x48
#define ACK_CMD 0x4F
#define RIO_CMD 0xFA
#define SHR_CMD 0xFB
#define TER_CMD 0xFC
#define CAE_CMD 0xFD
#define DHR_CMD 0xFE

#define ONE_DEVICE_TIMEOUT 5
#define MULTI_DEVICE_TIMEOUT 10
#define ONE_DEVICE_DATA_TIMEOUT 10
#define MULTI_DEVICE_DATA_TIMEOUT 16
#define SELF_TEST_TIMEOUT 200

typedef enum {
  HARD_RESET,
  SOFT_RESET,
  DHR_WAIT,
  IFC_WAIT,
  ACF_WAIT,
  IDD_WAIT,
  EXD_WAIT,
  EPT_WAIT,
  ELB_WAIT,
  RPL_WAIT,
  POL_WAIT,
  POLL_DELAY,
  PRM_WAIT,
  ACK_WAIT,
  RSC_WAIT
} state_t;

#define NON_FRAME 0x8000
const uint16_t WAITING = NON_FRAME + 0;
const uint16_t TIMEOUT = NON_FRAME + 1;
const uint16_t FRAME_ERROR = NON_FRAME + 2;
const uint16_t PARITY_ERROR = NON_FRAME + 3;
const uint16_t COMMAND_MISMATCH = NON_FRAME + 4;

inline bool is_frame(uint16_t frame) {
  return (frame & NON_FRAME) == 0;
}

inline bool frame_is_command(uint16_t frame) {
  return ((frame & (1 << 10)) != 0);
}

inline uint8_t frame_address(uint16_t frame) {
  return (frame >> 11) & 0x07;
}

inline uint8_t frame_data(uint16_t frame) {
  return (frame >> 2) & 0xFF;
}

inline uint8_t frame_command_op(uint16_t frame) {
  return frame_data(frame);
}

static uint8_t num_devices;
static state_t state;
static uint8_t wait_command;
static uint16_t wait_start_time;
static uint8_t wait_timeout;

static uint8_t data_address;
static uint8_t data_record[16];
static uint8_t data_index;
static uint16_t last_poll_time;

void send_command_and_wait(uint8_t address, uint8_t op, state_t new_state, uint8_t timeout); // Arduino bug
void send_command_and_wait(uint8_t address, uint8_t op, state_t new_state, uint8_t timeout) {
  frame_send_command(address, op);
  state = new_state;
  wait_command = op;
  wait_start_time = timer_read();
  wait_timeout = timeout;
  data_address = address;
  data_index = 0;
}

uint16_t recv_wait_frame(void) {
  if (queue_is_empty()) {
    if (timer_read() - wait_start_time > wait_timeout) {
      dprint("TO ");
      return TIMEOUT;
    } else {
      return WAITING;
    }
  } else {
    uint16_t frame = queue_remove();
    if (!(((frame & (1 << 14)) == 0) && ((frame & 1) == 1))) {
      // Start bit low; stop bit high.
      dprintf("Frame error: %04X\n", frame);
      return FRAME_ERROR;
    }
    uint8_t parity = 0;
    for (uint8_t i = 0; i < 15; i++) {
      if ((frame & (1 << i)) != 0) {
        parity ^= 1;
      }
    }
    if (!parity) {
      // When idle, the SLC will broadcast POL and receive it back without any data frame.
      // This has opcode 10; so with the command and stop bits and with no parity needed,
      // normal traffic is 441 both ways.
      dprintf("Parity error: %04X\n", frame);
      return PARITY_ERROR;
    }
#if 0
    dprintf("Received frame: %04X\n", frame);
#endif
    if (((frame>>2)&0xFF) != POL_CMD)
        dprintf("%d%s%02X ", (frame>>11)&0x7, (frame&(1<<10) ? "c" : "d"), (frame>>2)&0xFF);

    if (frame_is_command(frame)) {
      uint8_t op = frame_command_op(frame);
      if (wait_command == ACF_CMD) {
        op &= 0xF8;
      } else if (wait_command == POL_CMD || wait_command == RPL_CMD) {
        op &= 0xF0;
      }
      if (op != wait_command) {
        dprintf("Command mismatch: %04X\n", frame);
        return COMMAND_MISMATCH;
      }
    }
    return frame;
  }
}

bool data_record_add(uint16_t frame) {
  if (data_address == 0) {
    data_address = frame_address(frame);
  } else if (data_address != frame_address(frame)) {
    dprintln("Data address mismatch");
    return false;
  }

  if (data_index >= 16) {
    dprintln("Data record overflow");
    return false;
  }
//dprintf("R%d:[%02X] ", data_address, frame_data(frame));
  data_record[data_index++] = frame_data(frame);
  return true;
}

typedef struct {
  uint8_t device_id;
  unsigned has_extended_describe:1;
  unsigned has_report_name:1;
  unsigned has_security_code:1;
  unsigned axes:2;
  unsigned absolute_position:1;
  unsigned two_byte_position:1;
  unsigned general_prompt:1;
  unsigned prompts:3;
  unsigned buttons:3;
  uint16_t resolution;
  uint16_t max_count_x;
  uint16_t max_count_y;
  uint16_t max_count_z;
} device_info_t;

static device_info_t device_infos[7];

bool device_describe(void) {
  if (data_address < 1 || data_address > 7) {
    return false;
  }
  device_info_t *p_device = device_infos + (data_address - 1);
  uint8_t index = 0;
  if (index >= data_index) {
    return false;
  }
  p_device->device_id = data_record[index++];
  if (index >= data_index) {
    return false;
  }
  uint8_t record_header = data_record[index++];
  p_device->has_extended_describe = (record_header & (1 << 3)) != 0;
  p_device->has_security_code = (record_header & (1 << 2)) != 0;
  p_device->axes = record_header & 0x03;
  p_device->absolute_position = (record_header & (1 << 6)) != 0;
  p_device->two_byte_position = (record_header & (1 << 5)) != 0;
  if (p_device->axes) {
    p_device->resolution = (uint16_t)(data_record[index]|data_record[index+1]<<8);
    xprintf("res:%d ", p_device->resolution);
    index += 2;
  }
  if (p_device->absolute_position) {
    switch (p_device->axes) {
      case 3:
        p_device->max_count_z = (uint16_t)(data_record[index+4]|data_record[index+5]<<8);
        /* FALLTHRU */
      case 2:
        p_device->max_count_y = (uint16_t)(data_record[index+2]|data_record[index+3]<<8);
        /* FALLTHRU */
      case 1:
        p_device->max_count_x = (uint16_t)(data_record[index]|data_record[index+1]<<8);
        /* FALLTHRU */
    }
    xprintf("max_x:%d max_y:%d max_z:%d ", p_device->max_count_x, p_device->max_count_y, p_device->max_count_z);
    index += p_device->axes * 2;
  }
  if (index >= data_index) {
    return p_device->has_extended_describe;
  }
  if ((record_header & (1 << 4)) != 0) {
    uint8_t descriptor = data_record[index++];
    p_device->general_prompt = (descriptor >> 7) & 0x01;
    p_device->prompts = (descriptor >> 4) & 0x07;
    p_device->buttons = descriptor & 0x07;
  } else {
    p_device->buttons = 0;
  }
  return p_device->has_extended_describe;
}

void device_print(void) {
  if (data_address < 1 || data_address > 7) {
    return;
  }
  device_info_t *p_device = device_infos + (data_address - 1);
  xprintf("Addr:%d ID:%02X", data_address, p_device->device_id);
  if (p_device->has_security_code) {
    print(", security code");
  }
  if (p_device->general_prompt) {
    print(", general prompt");
  }
  if (p_device->prompts > 0) {
    xprintf(", prompts %d", p_device->prompts);
  }
  if (p_device->axes > 0) {
    xprintf(", axes %d", p_device->axes);
    if (p_device->absolute_position) {
      print(", absolute");
    }
    if (p_device->two_byte_position) {
      print(", 16-bit");
    }
  }
  if (p_device->buttons > 0) {
    xprintf(", buttons %d", p_device->buttons);
  }
  println("");
}

void device_extended_describe(void) {
  if (data_address < 1 || data_address > 7) {
    return;
  }
  device_info_t *p_device = device_infos + (data_address - 1);
  uint8_t index = 0;
  if (index >= data_index) {
    return;
  }
  uint8_t record_header = data_record[index++];
  p_device->has_report_name = (record_header & (1 << 4)) != 0;
}

void device_security_code(void) {
  if (data_index == 9) {
    if (data_record[0] == 0x10) {
      uint32_t product_number = data_record[1] | ((uint32_t)data_record[2] << 8) | (((uint32_t)data_record[3] >> 7) & 1) << 16;
      char product_letter = data_record[3] & 0x7F;
      uint32_t serial_number = data_record[4] | ((uint32_t)data_record[5] << 8) | ((uint32_t)data_record[6] << 16) | ((uint32_t)data_record[7] << 24);
      char serial_letter = data_record[8] & 0x7F;
      xprintf("%d: %05ld%c %04ld%c%05ld\n", data_address, product_number, product_letter, serial_number / 100000, serial_letter, serial_number % 100000);
      return;
    }
    if (data_record[0] == 0x00) {
      xprintf("%d: Empty security code report\n", data_address);
      return;
    }
  }
  xprintf("%d: Security code report not in expected format\n", data_address);
}

static inline int8_t range127(int16_t x) {
  if (x < -127) {
    return -127;
  } else if (x > 127) {
    return 127;
  } else {
    return (int8_t)x;
  }
}

void device_poll_record(void) {
  if (data_address < 1 || data_address > 7) {
    return;
  }
  device_info_t *p_device = device_infos + (data_address - 1);

  uint8_t index = 0;
  if (index >= data_index) {
    return;
  }
  uint8_t record_header = data_record[index++];

  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;
  do {
    uint8_t axes = record_header & 0x03;
    if (axes > 0) {
      if (p_device->two_byte_position) {
        if (index + 1 >= data_index) {
          break;
        }
        x = (int16_t)(data_record[index] | data_record[index + 1]);
        index += 2;
      } else {
        if (index >= data_index) {
          break;
        }
        x = (int8_t)data_record[index++];
      }
      if (axes > 1) {
        if (p_device->two_byte_position) {
          if (index + 1 >= data_index) {
            break;
          }
          y = (int16_t)(data_record[index] | data_record[index + 1]);
          index += 2;
        } else {
          if (index >= data_index) {
            break;
          }
          y = (int8_t)data_record[index++];
        }
        if (axes > 2) {
          if (p_device->two_byte_position) {
            if (index + 1 >= data_index) {
              break;
            }
            z = (int16_t)(data_record[index] | data_record[index + 1]);
            index += 2;
          } else {
            if (index >= data_index) {
              break;
            }
            z = (int8_t)data_record[index++];
          }
        }
      }
    }
  } while (false);

  if (x | y | z) {
    dprintf("[X:%d Y:%d Z:%d]\n", x, y, z);
  }

  uint8_t charset = (record_header >> 4) & 0x0F;
  uint8_t keyset = 0;
  switch (charset) {
  case 0:
    break;
  case 1:
  case 2:
  case 3:
  case 5:
      while (index < data_index) {
        // TODO: What devices send this?
        dprintf("%d: char %d data %02X\n", data_address, charset, data_record[index++]);
      }
      break;
  case 4:
    keyset = 1;
    break;
  case 6:
    keyset = 2;
    break;
  case 7:
    keyset = 3;
    break;
  }
  if (keyset != 0) {
    while (index < data_index) {
      uint8_t key_code = data_record[index++];
      uint8_t col = (key_code >> 1) & 0x0F;
      //uint8_t row = ((keyset - 1) << 3) | ((key_code >> 5) & 0x07);
      uint8_t row = ((key_code >> 5) & 0x07);
      if ((key_code & 1) == 0) {
dprintf("[D%02X] ", key_code);
        matrix[row] |= (1 << col);
      } else {
dprintf("[U%02X] ", key_code);
        matrix[row] &= ~(1 << col);
      }
    }
  }

#ifdef MOUSE_ENABLE
    static report_mouse_t mouse_report = {};
    uint8_t buttons = 0;
/*
  if (p_device->device_id == 0x68) { // HP Mouse
    // A mouse.
    // Translate BUTTON key states (80-8C) into mouse report button states and report if changed.
    buttons = (uint8_t)matrix[4];
    if (p_device->buttons > 2) {
      // The middle button is MOUSE_BTN3.
      if (((buttons >> 1) & 1) != ((buttons >> 2) & 1)) {
        buttons ^= 6;
      }
    }
  }
*/
    if (buttons != mouse_report.buttons || x != 0 || y != 0 || z != 0) {
      mouse_report.buttons = buttons;
      mouse_report.x = range127(x);
      mouse_report.y = range127(- y); // Also reverse direction.
      mouse_report.v = range127(z);
      //dprintf("[M: %02X, %d, %d, %d] ", mouse_report.buttons, mouse_report.x, mouse_report.y, mouse_report.v);
      host_mouse_send(&mouse_report);
    }
#endif
}

#define POLL_INTERVAL 16

static bool prompt_on = false;
static bool prompt_changed = false;
static uint8_t security_pending_address = 0;

static void frame_loop(void) {
  uint16_t frame;

  switch (state) {
  case HARD_RESET:
    LED_PORT |= LED_MASK;
    queue_clear();
    dprint("DHR: ");
    send_command_and_wait(0, DHR_CMD, DHR_WAIT, SELF_TEST_TIMEOUT);
    num_devices = 1;
    return;

  case SOFT_RESET:
    LED_PORT |= LED_MASK;
    queue_clear();
    dprint("DSR: ");
    send_command_and_wait(0, DSR_CMD, DHR_WAIT, MULTI_DEVICE_TIMEOUT);
    num_devices = 0;
    return;

  case POLL_DELAY:
    if (timer_read() - last_poll_time > POLL_INTERVAL) {
      send_command_and_wait(0, POL_CMD, POL_WAIT, MULTI_DEVICE_DATA_TIMEOUT);
      last_poll_time = wait_start_time;
    } else if (!queue_is_empty()) {
      state = SOFT_RESET;
    } else if (prompt_changed) {
      if (prompt_on) {
        dprint("PRM: ");
        send_command_and_wait(0, PRM_CMD, PRM_WAIT, MULTI_DEVICE_TIMEOUT);
      } else {
        dprint("ACK: ");
        send_command_and_wait(0, ACK_CMD, ACK_WAIT, MULTI_DEVICE_TIMEOUT);
      }
      prompt_changed = false;
    } else if (security_pending_address != 0) {
      dprint("RSC: ");
      send_command_and_wait(security_pending_address, RSC_CMD, RSC_WAIT, ONE_DEVICE_DATA_TIMEOUT);
      security_pending_address = 0;
    }
    return;

  case DHR_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (frame == TIMEOUT) {
      dprint("IFC: ");
      send_command_and_wait(0, IFC_CMD, IFC_WAIT, MULTI_DEVICE_TIMEOUT);
      return;
    } else if (is_frame(frame)) {
      dprintf("Spurious response to DHR: %02X\n", frame);
    }
    break;

  case IFC_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (frame == TIMEOUT) {
#if 0
      // This is what the document describes, but I don't see how it can avoid failing to identify the first device.
      if (num_devices >= 1) {
        dprint("ELB: ");
        send_command_and_wait(num_devices, ELB_CMD, ELB_WAIT, ONE_DEVICE_TIMEOUT);
        return;
      }
#endif
      dprintln("No response to IFC");
    } else if (is_frame(frame) && frame_is_command(frame)) {
      dprint("ACF: ");
      send_command_and_wait(0, ACF_CMD + 1, ACF_WAIT, MULTI_DEVICE_TIMEOUT);
      wait_command = ACF_CMD;
      return;
    }
    break;

  case ACF_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame) && frame_is_command(frame)) {
      num_devices = frame_command_op(frame) - ACF_CMD - 1;
      dprintf("%d device(s)\n", num_devices);
      dprint("IDD: ");
      send_command_and_wait(num_devices, IDD_CMD, IDD_WAIT, ONE_DEVICE_DATA_TIMEOUT);
      return;
    }
    state = SOFT_RESET;
    return;

  case IDD_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame)) {
      if (frame_is_command(frame)) {
        bool extended = device_describe();
        if (debug_enable) {
          device_print();
        }
        if (extended) {
          // Has extended describe.
          dprint("EXD: ");
          send_command_and_wait(num_devices, EXD_CMD, EXD_WAIT, ONE_DEVICE_DATA_TIMEOUT);
        } else {
          dprint("EPT: ");
          send_command_and_wait(num_devices, EPT_CMD, EPT_WAIT, ONE_DEVICE_TIMEOUT);
        }
        return;
      } else if (data_record_add(frame)) {
        return;
      }
    }
    state = SOFT_RESET;
    return;

  case EXD_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame)) {
      if (frame_is_command(frame)) {
        device_extended_describe();
        dprint("EPT: ");
        send_command_and_wait(num_devices, EPT_CMD, EPT_WAIT, ONE_DEVICE_TIMEOUT);
        return;
      } else if (data_record_add(frame)) {
        return;
      }
    }
    state = SOFT_RESET;
    return;

  case EPT_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame) && frame_is_command(frame)) {
      dprint("IFC: ");
      send_command_and_wait(0, IFC_CMD, IFC_WAIT, MULTI_DEVICE_TIMEOUT);
    } else {
      dprint("ELB: ");
      send_command_and_wait(num_devices, ELB_CMD, ELB_WAIT, ONE_DEVICE_TIMEOUT);
    }
    return;

  case ELB_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame) && frame_is_command(frame)) {
      dprint("RPL: ");
      send_command_and_wait(0, RPL_CMD, RPL_WAIT, MULTI_DEVICE_DATA_TIMEOUT);
      return;
    }
    state = SOFT_RESET;
    return;

  case RPL_WAIT:
  case POL_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame)) {
      if (frame_is_command(frame)) {
        device_poll_record();
        if (state == POL_WAIT) {
          LED_PORT &= ~LED_MASK;
          state = POLL_DELAY;
        } else {
          send_command_and_wait(0, POL_CMD, POL_WAIT, MULTI_DEVICE_DATA_TIMEOUT);
          last_poll_time = wait_start_time;
        }
        return;
      } else {
        if (data_address != 0 && data_address != frame_address(frame)) {
          device_poll_record();
          data_address = data_index = 0; // Start over for new device.
        }
        if (data_record_add(frame)) {
          return;
        }
      }
    }
    if (state == POL_WAIT) {
      state = SOFT_RESET;
      return;
    }
    break;

  case PRM_WAIT:
  case ACK_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame) && frame_is_command(frame)) {
      state = POLL_DELAY;
      return;
    }
    state = SOFT_RESET;
    return;

  case RSC_WAIT:
    frame = recv_wait_frame();
    if (frame == WAITING) {
      return;
    } else if (is_frame(frame)) {
      if (frame_is_command(frame)) {
        device_security_code();
        state = POLL_DELAY;
        return;
      } else if (data_record_add(frame)) {
        return;
      }
    }
    state = SOFT_RESET;
    return;

  default:
    dprintf("Unknown state: %d\n", state);
    break;
  }

  // Serious errors: start over.
  state = HARD_RESET;
}

void matrix_init(void) {
  debug_enable = true;

  SI_DDR &= ~SI_MASK;           // Input
  SI_PORT |= SI_MASK;           // Pullup to idle high
  SO_DDR |= SO_MASK;            // Output
  SO_PORT |= SO_MASK;           // Idle high

  // Interrupt 0 on falling edge.
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC01);

  LED_DDR |= LED_MASK;
  LED_PORT |= LED_MASK;

  state = HARD_RESET;
  num_devices = 0;
}

uint8_t matrix_scan(void) {
  frame_loop();
  //matrix_scan_quantum();
  return 1;
}

/*
void matrix_print(void) {
  print("\nr/c 02468ACE\n");
  for (uint8_t row = 0; row < MATRIX_ROWS * 2; row++) {
    // Keycode set numbers begin at 1.
    phex(row + 0x10); print(": ");
    // Each matrix row packs two keycode rows.
    print_bin_reverse8((uint8_t)(matrix_get_row(row / 2) >> ((row & 1) * 8)));
    print("\n");
  }
}
*/

inline
matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}

void led_set(uint8_t usb_led) {
  prompt_on = !!(usb_led & (1<<USB_LED_CAPS_LOCK));
  prompt_changed = true;
}

void command_security_id(void) {
  // Each command we find the next suitable device, cycling back and the end.
  static uint8_t next_address = 1;
  uint8_t address = next_address;
  do {
    device_info_t *p_device = device_infos + (address - 1);
    uint8_t new_address = address + 1;
    if (new_address > 7) {
      new_address = 1;
    }
    if (p_device->has_security_code) {
      next_address = new_address;
      security_pending_address = address;
      return;
    }
    address = new_address;
  } while (address != next_address);
  println("No devices with security id found.");
}

bool command_extra(uint8_t code) {
  switch (code) {
  case KC_I:
    command_security_id();
    return true;
  }
  return false;
}
