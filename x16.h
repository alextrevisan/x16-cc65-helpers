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

typedef void irqfunction(void);

void initialize(irqfunction *irq)
{
    // switch back to uppercase character set
    __asm__("lda #$8e");
    __asm__("jsr BSOUT");

    // disable interrupts
    __asm__("sei");

    // bad hack: redefine CC65 stack from $0xa800-0xaff, should be a proper x16 custom target
    *((uint8_t*) 0x02) = 0x00;
    *((uint8_t*) 0x03) = 0xb0;
    if (irq)
    {
        *((uint8_t*) 0x0314) = (uint8_t) (((uint16_t) irq) & 0xff);
        *((uint8_t*) 0x0315) = (uint8_t) ((((uint16_t) irq) >> 8) & 0xff);
    }

    // set new interrupt function
    //*((uint8_t*) 0x0314) = (uint8_t) (((uint16_t) irq) & 0xff);
    //*((uint8_t*) 0x0315) = (uint8_t) ((((uint16_t) irq) >> 8) & 0xff);


// enable interrupts
    __asm__("cli");
}

#endif // X16_H
