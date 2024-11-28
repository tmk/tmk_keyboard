#!/usr/bin/python3
# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
#
# This converts pin capture data into VCD format
#
# Capture record format:
#
#   P: pin state(4-bit)
#   TTT: timer[us](0-4095)
#   CC: timer overflow count(1-255)
#
#   1. Pin change:      TTTP
#   pin state should be changed from state of previous record.
#   TTT indicates time(12-bit):     0-FFFh(4095)
#
#   2. Timer overflow count:    CC0P
#   pin state should be same as state of previous record.
#   CC indicates time elapsed:      CC(1-255) * 1000h(4096)
#
#   3. Timer overflow count rollover:    000P
#   pin state should be same as state of previous record.
#   this indicates time elapsed:    256 * 1000h(4096), around 1sec
#

import sys
import re

# https://github.com/westerndigitalcorporation/pyvcd
from vcd import VCDWriter

if len(sys.argv) == 2:
    file = open(sys.argv[1])
else:
    file = sys.stdin
    print(f"Usage: {sys.argv[0]} [infile]", file=sys.stderr)
    print('or give data into stdin. Reading from stdin...', file=sys.stderr);

# write VCD
from datetime import datetime, timezone
with VCDWriter(sys.stdout, timescale='1 us', date=datetime.now(timezone.utc).ctime()) as writer:

    # prams: scope, name, var_type, size
    port = writer.register_var('tmk_capture', 'port', 'wire', size=4, init=0x0)

    # record: TTTP/CC0P/000P
    p = re.compile(r'([0-9A-Fa-f]{4})')

    ext_time = 0
    prv_time = 0
    prv_stat = -1
    for line in file:
        for record in line.strip().split():
            m = p.match(record)
            if m is not None:
                time = int(record[0:3], 16)
                stat = int(record[3:4], 16)

                if prv_stat == stat:
                    # no pin change: timer overflow count
                    if time == 0:
                        # overflow count rollover: 000P
                        ext_time += 0x100
                    else:
                        # overflow count: CC0P
                        ext_time += time >> 4

                    # no VCD record
                    #print('time overflow: ', time >> 4, hex(time), stat, file=sys.stderr);

                    prv_time = 0
                else:
                    # Pin change: TTTP
                    if prv_time >= time:
                        # time flipped
                        ext_time += 1
                        #print('time flipped: ', ext_time, ': ', hex(prv_time), '>', hex(time), file=sys.stderr);

                    # write VCD record
                    writer.change(port, ((ext_time * 0x1000) + time), stat)
                    #print(ext_time, time, hex(time), stat, file=sys.stderr)

                    prv_time = time
                    prv_stat = stat
            else:
                print('Invalid record: ', record, file=sys.stderr);
                #sys.exit(1)
