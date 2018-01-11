/**************************************************************************/
/*!
    @file     Adafruit_BLEGatt.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifndef _ADAFRUIT_BLEGATT_H_
#define _ADAFRUIT_BLEGATT_H_

#include <Arduino.h>
#include "Adafruit_BLE.h"

#define GATT_CHARS_PROPERTIES_BROADCAST       bit(0)
#define GATT_CHARS_PROPERTIES_READ            bit(1)
#define GATT_CHARS_PROPERTIES_WRITE_WO_RESP   bit(2)
#define GATT_CHARS_PROPERTIES_WRITE           bit(3)
#define GATT_CHARS_PROPERTIES_NOTIFY          bit(4)
#define GATT_CHARS_PROPERTIES_INDICATE        bit(5)

struct GattPresentationFormat
{
  uint8_t  format;
  int8_t   exponent;
  uint16_t unit;
  uint8_t  name_space;
  uint16_t desc;
};

class Adafruit_BLEGatt
{
private:
  Adafruit_BLE& _ble;

  uint8_t addChar_internal(uint8_t uuid[], uint8_t uuid_len, uint8_t properties, uint8_t min_len, uint8_t max_len, BLEDataType_t datatype, const char* description, const GattPresentationFormat* presentFormat);

public:
  char* buffer; // alias to ble's buffer

  Adafruit_BLEGatt(Adafruit_BLE& ble);

  bool    clear(void);

  uint8_t addService(uint16_t uuid16);
  uint8_t addService(uint8_t uuid128[]);

  uint8_t addCharacteristic(uint16_t uuid16  , uint8_t properties, uint8_t min_len, uint8_t max_len, BLEDataType_t datatype, const char* description = NULL, const GattPresentationFormat* presentFormat = NULL);
  uint8_t addCharacteristic(uint8_t uuid128[], uint8_t properties, uint8_t min_len, uint8_t max_len, BLEDataType_t datatype, const char* description = NULL, const GattPresentationFormat* presentFormat = NULL);

  //------------- Get Characteristic -------------//
  uint8_t  getChar(uint8_t charID);
  uint8_t  getChar(uint8_t charID, uint8_t* buf, uint8_t bufsize);

  uint8_t  getCharInt8(uint8_t charID)
  {
    if ( this->getChar(charID) < sizeof(uint8_t) ) return 0;
    uint8_t result;
    memcpy(&result, this->buffer, sizeof(result));
    return result;
  }

  uint16_t getCharInt16(uint8_t charID)
  {
    if ( this->getChar(charID) < sizeof(uint16_t) ) return 0;
    uint16_t result;
    memcpy(&result, this->buffer, sizeof(result));
    return result;
  }

  uint32_t getCharInt32(uint8_t charID)
  {
    if ( this->getChar(charID) < sizeof(uint32_t) ) return 0;
    uint32_t result;
    memcpy(&result, this->buffer, sizeof(result));
    return result;
  }

  char*    getCharStr(uint8_t charID)
  {
    if ( this->getChar(charID) == 0 ) return NULL;
    return this->buffer;
  }

  //------------- Set Characteristic -------------//
  bool    setChar(uint8_t charID, uint8_t const data[], uint8_t size);
  bool    setChar(uint8_t charID, char const *  str);

  bool    setChar(uint8_t charID, uint8_t  data8 ) { return this->setChar(charID, (uint8_t*) &data8, 1); }
  bool    setChar(uint8_t charID, int8_t   data8 ) { return this->setChar(charID, (uint8_t*) &data8, 1); }

  bool    setChar(uint8_t charID, uint16_t data16) { return this->setChar(charID, (uint8_t*) &data16, 2); }
  bool    setChar(uint8_t charID, int16_t  data16) { return this->setChar(charID, (uint8_t*) &data16, 2); }

  bool    setChar(uint8_t charID, uint32_t data32) { return this->setChar(charID, (uint8_t*) &data32, 4); }
  bool    setChar(uint8_t charID, int32_t  data32) { return this->setChar(charID, (uint8_t*) &data32, 4); }
};

enum
{
  GATT_PRESENT_FORMAT_BOOLEAN = 0x01,
  GATT_PRESENT_FORMAT_2BIT    = 0x02,
  GATT_PRESENT_FORMAT_4BIT    = 0x03,
  GATT_PRESENT_FORMAT_UINT8   = 0x04,
  GATT_PRESENT_FORMAT_UINT12  = 0x05,
  GATT_PRESENT_FORMAT_UINT16  = 0x06,
  GATT_PRESENT_FORMAT_UINT24  = 0x07,
  GATT_PRESENT_FORMAT_UINT32  = 0x08,
  GATT_PRESENT_FORMAT_UINT48  = 0x09,
  GATT_PRESENT_FORMAT_UINT64  = 0x0A,
  GATT_PRESENT_FORMAT_UINT128 = 0x0B,
  GATT_PRESENT_FORMAT_SINT8   = 0x0C,
  GATT_PRESENT_FORMAT_SINT12  = 0x0D,
  GATT_PRESENT_FORMAT_SINT16  = 0x0E,
  GATT_PRESENT_FORMAT_SINT24  = 0x0F,
  GATT_PRESENT_FORMAT_SINT32  = 0x10,
  GATT_PRESENT_FORMAT_SINT48  = 0x11,
  GATT_PRESENT_FORMAT_SINT64  = 0x12,
  GATT_PRESENT_FORMAT_SINT128 = 0x13,
  GATT_PRESENT_FORMAT_FLOAT32 = 0x14,
  GATT_PRESENT_FORMAT_FLOAT64 = 0x15,
  GATT_PRESENT_FORMAT_SFLOAT  = 0x16,
  GATT_PRESENT_FORMAT_FLOAT   = 0x17,
  GATT_PRESENT_FORMAT_DUINT16 = 0x18,
  GATT_PRESENT_FORMAT_UTF8S   = 0x19,
  GATT_PRESENT_FORMAT_UTF16S  = 0x1A,
  GATT_PRESENT_FORMAT_STRUCT  = 0x1B,
};

/* See https://developer.bluetooth.org/gatt/units/Pages/default.aspx */
enum
{
  GATT_PRESENT_UNIT_NONE                                                   = 0x2700,
  GATT_PRESENT_UNIT_LENGTH_METRE                                           = 0x2701,
  GATT_PRESENT_UNIT_MASS_KILOGRAM                                          = 0x2702,
  GATT_PRESENT_UNIT_TIME_SECOND                                            = 0x2703,
  GATT_PRESENT_UNIT_ELECTRIC_CURRENT_AMPERE                                = 0x2704,
  GATT_PRESENT_UNIT_THERMODYNAMIC_TEMPERATURE_KELVIN                       = 0x2705,
  GATT_PRESENT_UNIT_AMOUNT_OF_SUBSTANCE_MOLE                               = 0x2706,
  GATT_PRESENT_UNIT_LUMINOUS_INTENSITY_CANDELA                             = 0x2707,
  GATT_PRESENT_UNIT_AREA_SQUARE_METRES                                     = 0x2710,
  GATT_PRESENT_UNIT_VOLUME_CUBIC_METRES                                    = 0x2711,
  GATT_PRESENT_UNIT_VELOCITY_METRES_PER_SECOND                             = 0x2712,
  GATT_PRESENT_UNIT_ACCELERATION_METRES_PER_SECOND_SQUARED                 = 0x2713,
  GATT_PRESENT_UNIT_WAVENUMBER_RECIPROCAL_METRE                            = 0x2714,
  GATT_PRESENT_UNIT_DENSITY_KILOGRAM_PER_CUBIC_METRE                       = 0x2715,
  GATT_PRESENT_UNIT_SURFACE_DENSITY_KILOGRAM_PER_SQUARE_METRE              = 0x2716,
  GATT_PRESENT_UNIT_SPECIFIC_VOLUME_CUBIC_METRE_PER_KILOGRAM               = 0x2717,
  GATT_PRESENT_UNIT_CURRENT_DENSITY_AMPERE_PER_SQUARE_METRE                = 0x2718,
  GATT_PRESENT_UNIT_MAGNETIC_FIELD_STRENGTH_AMPERE_PER_METRE               = 0x2719,
  GATT_PRESENT_UNIT_AMOUNT_CONCENTRATION_MOLE_PER_CUBIC_METRE              = 0x271A,
  GATT_PRESENT_UNIT_MASS_CONCENTRATION_KILOGRAM_PER_CUBIC_METRE            = 0x271B,
  GATT_PRESENT_UNIT_LUMINANCE_CANDELA_PER_SQUARE_METRE                     = 0x271C,
  GATT_PRESENT_UNIT_REFRACTIVE_INDEX                                       = 0x271D,
  GATT_PRESENT_UNIT_RELATIVE_PERMEABILITY                                  = 0x271E,
  GATT_PRESENT_UNIT_PLANE_ANGLE_RADIAN                                     = 0x2720,
  GATT_PRESENT_UNIT_SOLID_ANGLE_STERADIAN                                  = 0x2721,
  GATT_PRESENT_UNIT_FREQUENCY_HERTZ                                        = 0x2722,
  GATT_PRESENT_UNIT_FORCE_NEWTON                                           = 0x2723,
  GATT_PRESENT_UNIT_PRESSURE_PASCAL                                        = 0x2724,
  GATT_PRESENT_UNIT_ENERGY_JOULE                                           = 0x2725,
  GATT_PRESENT_UNIT_POWER_WATT                                             = 0x2726,
  GATT_PRESENT_UNIT_ELECTRIC_CHARGE_COULOMB                                = 0x2727,
  GATT_PRESENT_UNIT_ELECTRIC_POTENTIAL_DIFFERENCE_VOLT                     = 0x2728,
  GATT_PRESENT_UNIT_CAPACITANCE_FARAD                                      = 0x2729,
  GATT_PRESENT_UNIT_ELECTRIC_RESISTANCE_OHM                                = 0x272A,
  GATT_PRESENT_UNIT_ELECTRIC_CONDUCTANCE_SIEMENS                           = 0x272B,
  GATT_PRESENT_UNIT_MAGNETIC_FLEX_WEBER                                    = 0x272C,
  GATT_PRESENT_UNIT_MAGNETIC_FLEX_DENSITY_TESLA                            = 0x272D,
  GATT_PRESENT_UNIT_INDUCTANCE_HENRY                                       = 0x272E,
  GATT_PRESENT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS               = 0x272F,
  GATT_PRESENT_UNIT_LUMINOUS_FLUX_LUMEN                                    = 0x2730,
  GATT_PRESENT_UNIT_ILLUMINANCE_LUX                                        = 0x2731,
  GATT_PRESENT_UNIT_ACTIVITY_REFERRED_TO_A_RADIONUCLIDE_BECQUEREL          = 0x2732,
  GATT_PRESENT_UNIT_ABSORBED_DOSE_GRAY                                     = 0x2733,
  GATT_PRESENT_UNIT_DOSE_EQUIVALENT_SIEVERT                                = 0x2734,
  GATT_PRESENT_UNIT_CATALYTIC_ACTIVITY_KATAL                               = 0x2735,
  GATT_PRESENT_UNIT_DYNAMIC_VISCOSITY_PASCAL_SECOND                        = 0x2740,
  GATT_PRESENT_UNIT_MOMENT_OF_FORCE_NEWTON_METRE                           = 0x2741,
  GATT_PRESENT_UNIT_SURFACE_TENSION_NEWTON_PER_METRE                       = 0x2742,
  GATT_PRESENT_UNIT_ANGULAR_VELOCITY_RADIAN_PER_SECOND                     = 0x2743,
  GATT_PRESENT_UNIT_ANGULAR_ACCELERATION_RADIAN_PER_SECOND_SQUARED         = 0x2744,
  GATT_PRESENT_UNIT_HEAT_FLUX_DENSITY_WATT_PER_SQUARE_METRE                = 0x2745,
  GATT_PRESENT_UNIT_HEAT_CAPACITY_JOULE_PER_KELVIN                         = 0x2746,
  GATT_PRESENT_UNIT_SPECIFIC_HEAT_CAPACITY_JOULE_PER_KILOGRAM_KELVIN       = 0x2747,
  GATT_PRESENT_UNIT_SPECIFIC_ENERGY_JOULE_PER_KILOGRAM                     = 0x2748,
  GATT_PRESENT_UNIT_THERMAL_CONDUCTIVITY_WATT_PER_METRE_KELVIN             = 0x2749,
  GATT_PRESENT_UNIT_ENERGY_DENSITY_JOULE_PER_CUBIC_METRE                   = 0x274A,
  GATT_PRESENT_UNIT_ELECTRIC_FIELD_STRENGTH_VOLT_PER_METRE                 = 0x274B,
  GATT_PRESENT_UNIT_ELECTRIC_CHARGE_DENSITY_COULOMB_PER_CUBIC_METRE        = 0x274C,
  GATT_PRESENT_UNIT_SURFACE_CHARGE_DENSITY_COULOMB_PER_SQUARE_METRE        = 0x274D,
  GATT_PRESENT_UNIT_ELECTRIC_FLUX_DENSITY_COULOMB_PER_SQUARE_METRE         = 0x274E,
  GATT_PRESENT_UNIT_PERMITTIVITY_FARAD_PER_METRE                           = 0x274F,
  GATT_PRESENT_UNIT_PERMEABILITY_HENRY_PER_METRE                           = 0x2750,
  GATT_PRESENT_UNIT_MOLAR_ENERGY_JOULE_PER_MOLE                            = 0x2751,
  GATT_PRESENT_UNIT_MOLAR_ENTROPY_JOULE_PER_MOLE_KELVIN                    = 0x2752,
  GATT_PRESENT_UNIT_EXPOSURE_COULOMB_PER_KILOGRAM                          = 0x2753,
  GATT_PRESENT_UNIT_ABSORBED_DOSE_RATE_GRAY_PER_SECOND                     = 0x2754,
  GATT_PRESENT_UNIT_RADIANT_INTENSITY_WATT_PER_STERADIAN                   = 0x2755,
  GATT_PRESENT_UNIT_RADIANCE_WATT_PER_SQUARE_METRE_STERADIAN               = 0x2756,
  GATT_PRESENT_UNIT_CATALYTIC_ACTIVITY_CONCENTRATION_KATAL_PER_CUBIC_METRE = 0x2757,
  GATT_PRESENT_UNIT_TIME_MINUTE                                            = 0x2760,
  GATT_PRESENT_UNIT_TIME_HOUR                                              = 0x2761,
  GATT_PRESENT_UNIT_TIME_DAY                                               = 0x2762,
  GATT_PRESENT_UNIT_PLANE_ANGLE_DEGREE                                     = 0x2763,
  GATT_PRESENT_UNIT_PLANE_ANGLE_MINUTE                                     = 0x2764,
  GATT_PRESENT_UNIT_PLANE_ANGLE_SECOND                                     = 0x2765,
  GATT_PRESENT_UNIT_AREA_HECTARE                                           = 0x2766,
  GATT_PRESENT_UNIT_VOLUME_LITRE                                           = 0x2767,
  GATT_PRESENT_UNIT_MASS_TONNE                                             = 0x2768,
  GATT_PRESENT_UNIT_PRESSURE_BAR                                           = 0x2780,
  GATT_PRESENT_UNIT_PRESSURE_MILLIMETRE_OF_MERCURY                         = 0x2781,
  GATT_PRESENT_UNIT_LENGTH_ANGSTROM                                        = 0x2782,
  GATT_PRESENT_UNIT_LENGTH_NAUTICAL_MILE                                   = 0x2783,
  GATT_PRESENT_UNIT_AREA_BARN                                              = 0x2784,
  GATT_PRESENT_UNIT_VELOCITY_KNOT                                          = 0x2785,
  GATT_PRESENT_UNIT_LOGARITHMIC_RADIO_QUANTITY_NEPER                       = 0x2786,
  GATT_PRESENT_UNIT_LOGARITHMIC_RADIO_QUANTITY_BEL                         = 0x2787,
  GATT_PRESENT_UNIT_LENGTH_YARD                                            = 0x27A0,
  GATT_PRESENT_UNIT_LENGTH_PARSEC                                          = 0x27A1,
  GATT_PRESENT_UNIT_LENGTH_INCH                                            = 0x27A2,
  GATT_PRESENT_UNIT_LENGTH_FOOT                                            = 0x27A3,
  GATT_PRESENT_UNIT_LENGTH_MILE                                            = 0x27A4,
  GATT_PRESENT_UNIT_PRESSURE_POUND_FORCE_PER_SQUARE_INCH                   = 0x27A5,
  GATT_PRESENT_UNIT_VELOCITY_KILOMETRE_PER_HOUR                            = 0x27A6,
  GATT_PRESENT_UNIT_VELOCITY_MILE_PER_HOUR                                 = 0x27A7,
  GATT_PRESENT_UNIT_ANGULAR_VELOCITY_REVOLUTION_PER_MINUTE                 = 0x27A8,
  GATT_PRESENT_UNIT_ENERGY_GRAM_CALORIE                                    = 0x27A9,
  GATT_PRESENT_UNIT_ENERGY_KILOGRAM_CALORIE                                = 0x27AA,
  GATT_PRESENT_UNIT_ENERGY_KILOWATT_HOUR                                   = 0x27AB,
  GATT_PRESENT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_FAHRENHEIT            = 0x27AC,
  GATT_PRESENT_UNIT_PERCENTAGE                                             = 0x27AD,
  GATT_PRESENT_UNIT_PER_MILLE                                              = 0x27AE,
  GATT_PRESENT_UNIT_PERIOD_BEATS_PER_MINUTE                                = 0x27AF,
  GATT_PRESENT_UNIT_ELECTRIC_CHARGE_AMPERE_HOURS                           = 0x27B0,
  GATT_PRESENT_UNIT_MASS_DENSITY_MILLIGRAM_PER_DECILITRE                   = 0x27B1,
  GATT_PRESENT_UNIT_MASS_DENSITY_MILLIMOLE_PER_LITRE                       = 0x27B2,
  GATT_PRESENT_UNIT_TIME_YEAR                                              = 0x27B3,
  GATT_PRESENT_UNIT_TIME_MONTH                                             = 0x27B4,
  GATT_PRESENT_UNIT_CONCENTRATION_COUNT_PER_CUBIC_METRE                    = 0x27B5,
  GATT_PRESENT_UNIT_IRRADIANCE_WATT_PER_SQUARE_METRE                       = 0x27B6
};

#endif /* _ADAFRUIT_BLEGATT_H_ */
