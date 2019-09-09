#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>

#include "initx16.h"
#include "x16.h"

void irq() {

}
static uint8_t colors[] = { 9, 6,  13};

int main()
{
    int colorloop;
    init();
    clrscr();
    mode80col();

    while (1)
    {
        for(colorloop = 0; colorloop < 16 ; colorloop++)
        {
            vpoke(0,colorloop*2,rand()%128);
            vpoke(0,colorloop*2+1,rand()%15);
        }
    }



    return 0;
}
