FC660C controller
=================
2017/02/24

Connector
---------
### Hirose DF14A-20P-1.25H
[datasheet](http://www.mouser.com/ds/2/185/DF14_catalog-939195.pdf)

### Pinouts

    |Controller |Switch  |Description                               |ATmega32u4
    |-----------|--------|------------------------------------------|--------------
    | 1         |  20    |    +Z7-LV07A-13-~CapsLED                 |
    | 2         |  19    |    +Z7-LV07A-11-~InsertLED               |
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
    |16         |   5    |     5V                                   |
    |17         |   4    |     3.3V                                 |
    |18         |   3    |     GND                                  |
    |19         |   2    |     GND                                  |
    |20         |   1    |     FG                                   |

```
* 5V intferface
+ 3.3V interface
```


Scan
----
Row: Drive(LVC138A)
Col: Sense(4051)
