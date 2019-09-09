#ifndef X16_H
#define X16_H

#include <stdint.h>

struct VERA_t
{
    uint8_t hi;
    uint8_t mid;
    uint8_t lo;
    uint8_t data1;
    uint8_t data2;
    uint8_t ctrl;
};

#define VERA (*(volatile struct VERA_t*) 0x9f20)

static void vpoke(uint8_t bank, uint16_t address, uint8_t data)
{
    // address selection 0
    VERA.ctrl = 0;
    // set address
    VERA.hi = bank;
    VERA.mid = address >> 8;
    VERA.lo = address & 0xff;
    // store data with data port 1
    VERA.data1 = data;
}

void clrscr()
{
    __asm__("lda #147");
    __asm__("jsr BSOUT");
}

void mode40col ()
{
    if(PEEK(0xD9) != 40)
    {
        __asm__("JSR $FF5F");
    }
}

void mode80col ()
{
    if(PEEK(0xD9) != 80)
    {
        __asm__("JSR $FF5F");
    }
}

#endif // X16_H
