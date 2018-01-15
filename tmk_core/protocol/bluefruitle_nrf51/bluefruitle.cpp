/*
Bluefruit Protocol for TMK firmware
Author: Ramon Poca <ramon.poca@gmail.com>
Based on code Copyright Benjamin Gould, 2013
Based on code Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdint.h>
#include "host.h"
#include "report.h"
#include "host_driver.h"
#include "bluefruitle.h"
#include "BluefruitConfig.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"


#define FACTORYRESET_ENABLE 0
#undef BLUEFRUITLE_TRACE_SERIAL 



const int debug_keyboard = 1;
static uint8_t bluefruitle_keyboard_leds = 0;

static Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


void dprintf(const char *c) {
    ble.print("AT+BLEUARTTX=");
    ble.print(c);
    ble.println("\\r\\n");
}

void debug_hex8(int i) {
    ble.print("AT+BLEUARTTX=");
    ble.print((unsigned char) i, HEX);
    ble.println("\\r\\n");
}
void debug_hex16(int i) {
    ble.print("AT+BLEUARTTX=");
    ble.print((short) i, HEX);
    ble.println("\\r\\n");
}

void bluefruitle_keyboard_print_report(report_keyboard_t *report)
{
    if (!debug_keyboard) 
        return;
    dprintf("keys: "); for (int i = 0; i < KEYBOARD_REPORT_KEYS; i++) { debug_hex8(report->keys[i]); dprintf(" "); }
    dprintf(" mods: "); debug_hex8(report->mods);
    dprintf(" reserved: "); debug_hex8(report->reserved); 
    dprintf("\n");
}

#ifdef BLUEFRUITLE_TRACE_SERIAL
static void bluefruitle_trace_header()
{
    dprintf("+------------------------------------+");
    dprintf("| HID report to Bluefruit via serial |");
    dprintf("+------------------------------------+|");
}

static void bluefruitle_trace_footer()
{
    dprintf("|+------------------------------------+");
}
#endif


/*------------------------------------------------------------------*
 * Host driver
 *------------------------------------------------------------------*/

static uint8_t keyboard_leds(void);
static void send_keyboard(report_keyboard_t *report);
static void send_mouse(report_mouse_t *report);
static void send_system(uint16_t data);
static void send_consumer(uint16_t data);

static host_driver_t driver = {
        keyboard_leds,
        send_keyboard,
        send_mouse,
        send_system,
        send_consumer
};


static bool isConnected = false;

static void connectedCB(void) {
  dprintf("CONNECTED");
  isConnected = true;
}

static void disconnectedCB(void) {
  dprintf("DISCONNECTED");
  isConnected = false;
}

void ble_task(void) {
    while (ble.available()) 
        ble.read();
}

#define _STR(x) #x
#define STR(x) _STR(x)

void ble_init(void) {
      if (!ble.begin(VERBOSE_MODE)) {
        dprintf( "Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?");
      }
      if ( FACTORYRESET_ENABLE) {
        /* Perform a factory reset to make sure everything is in a known state */
        dprintf("Performing a factory reset: ");
        if (!ble.factoryReset()) {
          dprintf("Couldn't factory reset");
        }
      }
      ble.echo(true);
      ble.info();
      ble.setConnectCallback(connectedCB);
      ble.setDisconnectCallback(disconnectedCB);
      ble.print("AT+GAPDEVNAME=");
      ble.println(STR(PRODUCT));
      ble.println("AT+BAUDRATE=9600");
      ble.println("AT+BLEHIDEN=1");
      ble.println("AT+BLEKEYBOARDEN=1");
      _delay_ms(100);
      ble.println("ATZ"); // Reset after enabling keyboard
      _delay_ms(1000);
      ble.verbose(false);
}

host_driver_t *bluefruitle_driver(void)
{
      return &driver;
}

static uint8_t keyboard_leds(void) {
    return bluefruitle_keyboard_leds;
}

static void send_keyboard(report_keyboard_t *report)
{
#ifdef BLUEFRUITLE_TRACE_SERIAL   
    bluefruitle_trace_header();
#endif
    char hexbuf[8];
    ble.print("AT+BLEKEYBOARDCODE=");
    for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
        snprintf(hexbuf, sizeof(hexbuf), "%02X", report->raw[i]);
        ble.print(hexbuf);
        if (i < KEYBOARD_REPORT_SIZE - 1)
            ble.print("-");
    }
    ble.println(""); 
#ifdef BLUEFRUITLE_TRACE_SERIAL   
    ble.print("AT+BLEUARTTX=");
    for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
        snprintf(hexbuf, sizeof(hexbuf), "%02X", report->raw[i]);
        ble.print(hexbuf);
        if (i < KEYBOARD_REPORT_SIZE - 1)
            ble.print("-");
    }
    ble.println(""); 
#endif
    
#ifdef BLUEFRUITLE_TRACE_SERIAL   
    bluefruitle_trace_footer();   
#endif
}

static void send_mouse(report_mouse_t *report)
{
#ifdef BLUEFRUITLE_TRACE_SERIAL   
    bluefruitle_trace_header();
#endif
#ifdef BLUEFRUITLE_TRACE_SERIAL
    bluefruitle_trace_footer();
#endif
}

static void send_system(uint16_t data)
{
}

/*
+-----------------+-------------------+-------+
| Consumer Key    | Bit Map           | Hex   |
+-----------------+-------------------+-------+
| Home            | 00000001 00000000 | 01 00 |
| KeyboardLayout  | 00000010 00000000 | 02 00 |
| Search          | 00000100 00000000 | 04 00 |
| Snapshot        | 00001000 00000000 | 08 00 |
| VolumeUp        | 00010000 00000000 | 10 00 |
| VolumeDown      | 00100000 00000000 | 20 00 |
| Play/Pause      | 01000000 00000000 | 40 00 |
| Fast Forward    | 10000000 00000000 | 80 00 |
| Rewind          | 00000000 00000001 | 00 01 |
| Scan Next Track | 00000000 00000010 | 00 02 |
| Scan Prev Track | 00000000 00000100 | 00 04 |
| Random Play     | 00000000 00001000 | 00 08 |
| Stop            | 00000000 00010000 | 00 10 |
+-------------------------------------+-------+
*/
#define CONSUMER2BLUEFRUITLE(usage) \
    (usage == AUDIO_MUTE              ? 0x0000  : \
    (usage == AUDIO_VOL_UP            ? 0x1000  : \
    (usage == AUDIO_VOL_DOWN          ? 0x2000  : \
    (usage == TRANSPORT_NEXT_TRACK    ? 0x0002  : \
    (usage == TRANSPORT_PREV_TRACK    ? 0x0004  : \
    (usage == TRANSPORT_STOP          ? 0x0010  : \
    (usage == TRANSPORT_STOP_EJECT    ? 0x0000  : \
    (usage == TRANSPORT_PLAY_PAUSE    ? 0x4000  : \
    (usage == APPLAUNCH_CC_CONFIG     ? 0x0000  : \
    (usage == APPLAUNCH_EMAIL         ? 0x0000  : \
    (usage == APPLAUNCH_CALCULATOR    ? 0x0000  : \
    (usage == APPLAUNCH_LOCAL_BROWSER ? 0x0000  : \
    (usage == APPCONTROL_SEARCH       ? 0x0400  : \
    (usage == APPCONTROL_HOME         ? 0x0100  : \
    (usage == APPCONTROL_BACK         ? 0x0000  : \
    (usage == APPCONTROL_FORWARD      ? 0x0000  : \
    (usage == APPCONTROL_STOP         ? 0x0000  : \
    (usage == APPCONTROL_REFRESH      ? 0x0000  : \
    (usage == APPCONTROL_BOOKMARKS    ? 0x0000  : 0)))))))))))))))))))

static void send_consumer(uint16_t data)
{
    static uint16_t last_data = 0;
    if (data == last_data) return;
    last_data = data;
    
    uint16_t bitmap = CONSUMER2BLUEFRUITLE(data);
    
#ifdef BLUEFRUITLE_TRACE_SERIAL   
    dprintf("\nData: "); 
    debug_hex16(data); 
    dprintf("; bitmap: "); 
    debug_hex16(bitmap); 
    dprintf("\n");
    bluefruitle_trace_header();
#endif
#ifdef BLUEFRUITLE_TRACE_SERIAL
    bluefruitle_trace_footer();
#endif
}

