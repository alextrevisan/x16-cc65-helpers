#ifndef INITX16_H
#define INITX16_H
#include <stdint.h>
static void irq();
static void __defined_irq() {
    irq();
    __asm__("PLA");
    __asm__("TAY");
    __asm__("PLA");
    __asm__("TAX");
    __asm__("PLA");
    __asm__("RTI");
}
void init() {
    // switch back to uppercase character set
    __asm__("lda #$8e");
    __asm__("jsr BSOUT");

    // disable interrupts
    __asm__("sei");

    // bad hack: redefine CC65 stack from $0xa800-0xaff, should be a proper x16 custom target
    *((uint8_t*) 0x02) = 0x00;
    *((uint8_t*) 0x03) = 0xb0;

    // set new interrupt function
    *((uint8_t*) 0x0314) = (uint8_t) (((uint16_t) __defined_irq) & 0xff);
    *((uint8_t*) 0x0315) = (uint8_t) ((((uint16_t) __defined_irq) >> 8) & 0xff);
    __asm__("cli");

}
#endif // INITX16_H
