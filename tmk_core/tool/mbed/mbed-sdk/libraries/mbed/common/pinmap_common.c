/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "pinmap.h"
#include "mbed_error.h"

void pinmap_pinout(PinName pin, const PinMap *map) {
    if (pin == NC)
        return;

    while (map->pin != NC) {
        if (map->pin == pin) {
            pin_function(pin, map->function);

            pin_mode(pin, PullNone);
            return;
        }
        map++;
    }
    error("could not pinout");
}

uint32_t pinmap_merge(uint32_t a, uint32_t b) {
    // both are the same (inc both NC)
    if (a == b)
        return a;

    // one (or both) is not connected
    if (a == (uint32_t)NC)
        return b;
    if (b == (uint32_t)NC)
        return a;

    // mis-match error case
    error("pinmap mis-match");
    return (uint32_t)NC;
}

uint32_t pinmap_find_peripheral(PinName pin, const PinMap* map) {
    while (map->pin != NC) {
        if (map->pin == pin)
            return map->peripheral;
        map++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_peripheral(PinName pin, const PinMap* map) {
    uint32_t peripheral = (uint32_t)NC;

    if (pin == (PinName)NC)
        return (uint32_t)NC;
    peripheral = pinmap_find_peripheral(pin, map);
    if ((uint32_t)NC == peripheral) // no mapping available
        error("pinmap not found for peripheral");
    return peripheral;
}

uint32_t pinmap_find_function(PinName pin, const PinMap* map) {
    while (map->pin != NC) {
        if (map->pin == pin)
            return map->function;
        map++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_function(PinName pin, const PinMap* map) {
    uint32_t function = (uint32_t)NC;

    if (pin == (PinName)NC)
        return (uint32_t)NC;
    function = pinmap_find_function(pin, map);
    if ((uint32_t)NC == function) // no mapping available
        error("pinmap not found for function");
    return function;
}
