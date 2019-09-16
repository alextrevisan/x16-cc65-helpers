#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>
#include "x16.h"

//void (*readkey)(void) = (void (*)(void)) 0xFF5F;
//void (*getkey)(void) = (void (*)(void)) 0xFFE4;
void (*printkey)(void) = (void (*)(void)) 0xFFD2;
//int8_t __fastcall__ strcmp2(char s1[], char s2[]);
//int strcmp (const char* s1, const char* s2);
int printvalue = 0;
char readtxt[32];
static uint8_t currentKey = 0;
void getkey()
{
    __asm__("JSR $FFE4");
    __asm__("STA %v", currentKey);
}
static void irq()
{
    // return from interrupt
    __asm__("PLA");
    __asm__("TAY");
    __asm__("PLA");
    __asm__("TAX");
    __asm__("PLA");
    __asm__("RTI");


}

int __fastcall__ strcmp2 (const char* s1, const char* s2);

void __fastcall__ printc(char)
{
    __asm__("JSR $FFD2");
    return;
}
void __fastcall__ print(char readtxt[])
{
    static uint8_t idx = 0;
    for(idx = 0; readtxt[idx]!='\0'; idx++)
    {
        printc(readtxt[idx]);
    }
}
void clearline()
{
    uint8_t line;
    for(line = 0; line < 32; line++)
    {
        readtxt[line] = '\0';
    }
}
/*int8_t __fastcall__ strcmp2(char s1[], char s2[])
{
    static int8_t s1s = 0;
    static int8_t s2s = 0;
    s1s = strlen(s1);
    s2s=strlen(s2);
    if(s1s != s2s)
    {
        return 0;
    }
    while(s1s >= 0)
    {
        if(s1[s1s]!=s2[s1s])
            return 0;
        s1s--;
    }
    return 1;
}*/
/*void scanline()
{
    static unsigned char c ='\0';
    static uint8_t idx = 0;
    while(c!=13)
    {
        c = getkey();
        if(c != 0)
        {
            readtxt[idx] = c;
            printc(readtxt[idx]);
            idx++;
        }

    }
    idx = 0;
    c = '\0';
}*/
#pragma static-locals(on)
#include "logMath.h"

#define KEYTURNLEFT 'q'
#define KEYTURNRIGHT 'e'
#define KEY_MOVELEFT 'a'
#define KEY_MOVERIGHT 'd'
#define KEY_FORWARD 'w'
#define KEY_BACK 's'

static int playeroldx=10240;
static int playeroldy=7680;
static int playerx = 10240;
static int playery = 7680;
static int16_t playera = 0;
static char turnLeftSpeed = 0;
static char turnRightSpeed = 0;
static int ca = 0, sa = 0;
static int oldca = 0, oldsa = 0;

static int vx0 = 70;
static int vx1 = 70;
static int vy0 = 20;
static int vy1 = 50;


int main()
{
    initialize(irq);

    for(;;)
    {
        getkey();
        if(currentKey == KEYTURNLEFT)
        {
            turnRightSpeed = 0;
            if (turnLeftSpeed < 3)
            {
                ++turnLeftSpeed;
            }
            playera -= turnLeftSpeed;
            //printf("%d\n", playera);
        }
        else if(currentKey == KEYTURNRIGHT)
        {
            turnLeftSpeed = 0;
            if (turnRightSpeed < 3)
            {
                ++turnRightSpeed;
            }
            playera += turnRightSpeed;
            //printf("%d\n", playera);
        }
        else
        {
            turnLeftSpeed = 0;
            turnRightSpeed = 0;
        }
        oldca = ca;
        oldsa = sa;
        playera = playera > 359 ? 0 : playera;
        playera = playera < 0 ? 359 : playera;
        setCameraAngle(playera);
        ca = cosa<<1;
        sa = sina<<1;
        playeroldx = playerx;
        playeroldy = playery;
        if (currentKey == KEY_MOVELEFT)
        {
            playerx -= sa;
            playery -= ca;

            //printf("%d, %d\n", playerx, playery);
        }
        if (currentKey == KEY_MOVERIGHT)
        {
            playerx += (sa<<1);
            playery += (ca<<1);

            //printf("%d, %d\n", playerx, playery);
        }

        if (currentKey == KEY_FORWARD)
        {
            //if (!(testFilled(0) < 4 && typeAtCenterOfView == TYPE_OBJECT))
            //{
            playerx += ca;
            playery -= sa;
            //}
                //printf("%d, %d\n", playerx, playery);
        }
        if (currentKey == KEY_BACK)
        {
            playerx -= ca;
            playery += sa;
            //printf("%d, %d\n", playerx, playery);
        }
        //cline(playerx/256, playery/256, 30, 30);
        line(vx0, vy0, vx1, vy1, 0x80+' ');

        line(playeroldx/256, playeroldy/256, (oldca*5+playeroldx)/256, (oldsa*5+playeroldy)/256, ' ');
        line(playerx/256, playery/256, (ca*5+playerx)/256, (sa*5+playery)/256, 0x80+' ');
    }

    return 0;
}
