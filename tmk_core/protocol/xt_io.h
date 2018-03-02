#ifndef XT_IO_H
#define XT_IO_H

#define XT_DATA_IN()        do { \
    XT_DATA_DDR  &= ~(1<<XT_DATA_BIT); \
    XT_DATA_PORT |=  (1<<XT_DATA_BIT); \
} while (0)

#define XT_DATA_READ()      (XT_DATA_PIN&(1<<XT_DATA_BIT))

#define XT_DATA_LO()        do { \
    XT_DATA_PORT &= ~(1<<XT_DATA_BIT); \
    XT_DATA_DDR  |=  (1<<XT_DATA_BIT); \
} while (0)


#define XT_CLOCK_IN()       do { \
    XT_CLOCK_DDR  &= ~(1<<XT_CLOCK_BIT); \
    XT_CLOCK_PORT |=  (1<<XT_CLOCK_BIT); \
} while (0)

#define XT_CLOCK_READ()     (XT_CLOCK_PIN&(1<<XT_CLOCK_BIT))

#define XT_CLOCK_LO()       do { \
    XT_CLOCK_PORT &= ~(1<<XT_CLOCK_BIT); \
    XT_CLOCK_DDR  |=  (1<<XT_CLOCK_BIT); \
} while (0)

#endif
