#include "action.h"
#include "actionmap.h"
#include "hook.h"
#include "fc660c.h"
#include "print.h"
#include "lib_i2c/i2c_master.h"

// !!!!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!!!!!
// Writing improper value to EEPROM may mess your keyboard and
// it will require some hassles to recover it.
//
// CHECK DATASHEETS SCHEMATICS AND CODES BEFORE TRYING.
// !!!!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!!!!!

// actuation point tune up and down
#define AC_ACUP     ACTION_FUNCTION(0)
#define AC_ACDN     ACTION_FUNCTION(1)
// print tolerance
#define AC_TOLE     ACTION_FUNCTION(2)
// restore RDAC from EEPROM
#define AC_RSTO     ACTION_FUNCTION(3)
// store RDAC to EEPROM
#define AC_STOR     ACTION_FUNCTION(4)
// write EEPROM test
#define AC_EPWR     ACTION_FUNCTION(5)
// enable write protect
#define AC_PROT     ACTION_FUNCTION(6)
// disable write protect
#define AC_UNPR     ACTION_FUNCTION(7)

/*
 * ,-----------------------------------------------------------.  ,---.
 * |ESC|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|   BSPC|  |   |
 * |-----------------------------------------------------------|  |---|
 * |TAB  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|   \ |  |   |
 * |-----------------------------------------------------------|  `---'
 * |LCTL  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|     ENT|
 * |--------------------------------------------------------------.
 * |LSFT    |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|     RSFT|   |
 * |------------------------------------------------------------------,
 * |LCTL|LGUI|LALT|          SPC          |RALT|RCTL|  Fn|    |   |   |
 * `------------------------------------------------------------------'
 *
 * Insert:  Actuation point up
 * Delete:  Actuation point down
 * Up:      Print tolerance
 * Down:    Restore RDAC(actuation point) from EEPROM
 * Left:    enable write protect
 * Right:   disable write protect
 */
const action_t actionmaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    [0] = KMAP(
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,     ACUP,
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS,     ACDN,
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT,TOLE,
        LCTL,LGUI,LALT,          SPC,                     RALT,RCTL,GRV, PROT,RSTO,UNPR
    ),
};



///////////////////////////////////////////////////////////////////////////////
//
// AD5258 I2C digital potentiometer
// http://www.analog.com/media/en/technical-documentation/data-sheets/AD5258.pdf
//
#define AD5258_ADDR 0b0011000
#define AD5258_WR   (((AD5258_ADDR)<<1) | 0)
#define AD5258_RD   (((AD5258_ADDR)<<1) | 1)
#define AD5258_INST_RDAC        0x00
#define AD5258_INST_EEPROM      0x20
#define AD5258_INST_PROTECT     0x40
#define AD5258_INST_NOP         0x80
#define AD5258_INST_RESTORE     0xA0
#define AD5258_INST_STORE       0xC0
#define AD5258_INST_TOLERANCE   0x3E


void hook_early_init(void)
{
    i2c_init();
}

uint8_t read_rdac(void) {
    // read RDAC register
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_RDAC);
    i2c_start(AD5258_RD);
    uint8_t ret = i2c_read_nack();
    i2c_stop();
    return ret;
}

void write_rdac(uint8_t rdac) {
    // write RDAC register:
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_RDAC);
    i2c_write(rdac & 0x3F);
    i2c_stop();
}

void actuation_point_up(void) {
    // write RDAC register: lower value makes actioation point shallow
    uint8_t rdac = read_rdac();
    write_rdac(rdac-1);
}

void actuation_point_down(void) {
    // write RDAC register: higher value makes actioation point deep
    uint8_t rdac = read_rdac();
    write_rdac(rdac+1);
}

uint16_t read_tolerance(void) {
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_TOLERANCE);
    i2c_start(AD5258_RD);
    uint8_t ret1 = i2c_read_ack();
    uint8_t ret2 = i2c_read_nack();
    i2c_stop();
    return (ret1 << 8) | ret2;
}

uint8_t read_eeprom(void) {
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_EEPROM);
    i2c_start(AD5258_RD);
    uint8_t ret = i2c_read_nack();
    i2c_stop();
    return ret;
}

void write_eeprom(uint8_t eeprom) {
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_EEPROM);
    i2c_write(eeprom & 0x3F);
    i2c_stop();
}

void write_protect(bool b) {
    // enable write protect when true or logic high
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_PROTECT);
    i2c_write(b ? 1 : 0);
    i2c_stop();
}

void store_to_eeprom(void) {
    // Store RDAC to EEPROM
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_STORE);
    i2c_stop();
}

void restore_from_eeprom(void) {
    // Restore RDAC from EEPROM
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_RESTORE);
    i2c_stop();
}

void nop_for_idle(void) {
    i2c_start(AD5258_WR);
    i2c_write(AD5258_INST_NOP);
    i2c_stop();
}
///////////////////////////////////////////////////////////////////////////////




void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    if (record->event.pressed) {

        switch (id) {
        case 0:
            xprintf("Action point up\n");
            actuation_point_up();
            break;
        case 1:
            xprintf("Action point down\n");
            actuation_point_down();
            break;
        case 2:
            xprintf("Tolerance: 0x%04X\n", read_tolerance());
            break;
        case 3:
            xprintf("Restore RDAC from EEPROM\n");
            restore_from_eeprom();
            nop_for_idle();
            break;
        case 4:
            xprintf("Store RDAC to EEPROM\n");
            store_to_eeprom();
            _delay_ms(30); // gives time to write EEPROM
            break;
        case 5:
            xprintf("Write EEPROM\n");
            write_eeprom(read_rdac());
            _delay_ms(30); // gives time to write EEPROM
        case 6:
            xprintf("enable write protect\n");
            write_protect(true);
            break;
        case 7:
            xprintf("disable write protect\n");
            write_protect(false);
            break;
        }

        xprintf("RDAC: 0x%02X, EEPROM: 0x%02X\n", read_rdac(), read_eeprom());
    }
}
