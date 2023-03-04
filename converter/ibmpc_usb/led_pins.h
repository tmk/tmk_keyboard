/*
Copyright 2023 an_achronism <87213873+an-achronism@users.noreply.github.com>

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

#pragma once

/*
The default pin assignments in this file have been chosen to retain full
backwards compatibility with Soarer's old converter solution:
https://geekhack.org/index.php?topic=17458.0
*/

// The Data Direction Register and port must match (default is F)
#define LOCK_INDICATOR_DDR   DDRF
#define LOCK_INDICATOR_PORT  PORTF

// These pin numbers correspond with the chosen port, e.g. port F pin 5 is PF5
#define CAPS_LOCK_PIN        5
#define NUM_LOCK_PIN         6
#define SCROLL_LOCK_PIN      7
