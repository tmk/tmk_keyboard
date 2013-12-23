/*
Author: Benjamin Gould

Based on code Copyright 2010,2011 Jun WAKO <wakojun@gmail.com>

Original code is Copyright 2013 Benjamin Gould (bgould on github.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef XT_H
#define XT_H

/*
Simple bitbang for XT protocol
*/

/* make sure clock and data lines are defined */

/* port settings for clock and data line */
#if !(defined(XT_CLOCK_PORT) && \
      defined(XT_CLOCK_PIN) && \
      defined(XT_CLOCK_DDR) && \
      defined(XT_CLOCK_BIT))
#   error "XT clock port setting is required in config.h"
#endif

#if !(defined(XT_DATA_PORT) && \
      defined(XT_DATA_PIN) && \
      defined(XT_DATA_DDR) && \
      defined(XT_DATA_BIT))
#   error "XT data port setting is required in config.h"
#endif

void xt_host_init(void);
uint8_t xt_host_recv(void);

#endif
