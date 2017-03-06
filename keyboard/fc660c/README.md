FC660C controller
=================
2017/02/24

Connector
---------
### Hirose DF14A-20P-1.25H
[datasheet](http://www.mouser.com/ds/2/185/DF14_catalog-939195.pdf)

### Pinouts
LV07A and LVC138A are 5V-tolerant, the keyboard will work only with 5V power.

    |Controller |Switch  |Description                               |ATmega32u4
    |-----------|--------|------------------------------------------|--------------
    | 1         |  20    |    +Z7-LV07A-13-~CapsLED                 |PB6
    | 2         |  19    |    +Z7-LV07A-11-~InsertLED               |PB7
    | 3         |  18    |    +Z3-LVC138A-4-~G2A  Z6-TP1684-5-~EN   |PD7
    | 4         |  17    |    +Z3-LVC138A-3-A                       |PD4
    | 5         |  16    |    +Z3-LVC138A-3-B                       |PD5
    | 6         |  15    |    +Z3-LVC138A-3-C                       |PD6
    | 7         |  14    |    +Z7-LV07A-3  LV4051A-11-A             |PB0
    | 8         |  13    |    +Z7-LV07A-1  LV4051A-10-B             |PB1
    | 9         |  12    |    +Z7-LV07A-5  LV4051A-9-C              |PB2
    |10         |  11    |    *Z5-LV4051A-6-~EN(Col 8-F)            |PB3
    |11         |  10    |    *Z4-LV4051A-6-~EN(Col 0-7)            |PB4
    |12         |   9    |    *Z2-AD5258-4-SDA(I2C)                 |PD1(TWI)
    |13         |   8    |    *Z2-AD5258-5-SDL(I2C)                 |PD0(TWI)
    |14         |   7    |    *Z6-TP1684-2-KEY(i)                   |PC6
    |15         |   6    |    *Z6-TP1684-4-HYS(o)                   |PC7
    |16         |   5    |     5V                                   |5V
    |17         |   4    |     3.3V                                 |5V
    |18         |   3    |     GND                                  |GND
    |19         |   2    |     GND                                  |GND
    |20         |   1    |     FG                                   |GND

```
* 5V intferface
+ 3.3V interface
```

Key Matrix
----------
Row5-7 has no key, you can omit scaning those. Topre original controller scans those rows for some reason.

    |    |0   |1   |2   |3   |4   |5   |6   |7   |8   |9   |A   |B   |C   |D   |E   |F   |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |   0|Q   |W   |E   |Tab |R   |U   |T   |Y   |O   |P   |[   |I   |]   |    |\   |Del |
    |   1|1   |2   |3   |Esc |4   |7   |5   |6   |9   |0   |-   |8   |=   |    |BSpc|Ins |
    |   2|    |LWin|LAlt|LCtl|    |    |    |Spc |RAlt|    |RCtl|    |Fn  |Down|Left|Rght|
    |   3|    |Z   |X   |LSft|C   |N   |V   |B   |,   |.   |/   |M   |RSft|Up  |    |    |
    |   4|A   |S   |D   |Caps|F   |J   |G   |H   |L   |;   |'   |K   |    |    |Entr|    |
    |   5|    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |   6|    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |   7|    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |


Scan
----
Row: Drive(LVC138A)
Col: Sense(4051)


Resources
---------
- Pics https://imgur.com/a/4UKkZ
