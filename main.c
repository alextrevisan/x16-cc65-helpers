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
/***/

static int playerx = 8*256;
static int playery = 8*256;
static int16_t playera = 0;
static char turnLeftSpeed = 0;
static char turnRightSpeed = 0;
static const int fDepth = 16*256;
static const char map[] =
{
"################"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"#              #"\
"################"\
};
static const int nScreenWidth = 80*256;
static const int nScreenHeight = 60*256;
static const int nMapWidth = 16*256;
static const int nMapHeight = 16*256;
//3.14159 * 256 = 804 / 4 = 201
static int fFOV = 201;
static int fRayAngle = 0;

static uint8_t mapIdx;
static int nCeiling;
static int nFloor;

int main()
{
    uint16_t x;
    uint16_t y;
    static long tmp;
    int fDistanceToWall;
    int nTestX, nTestY;
    uint8_t bHitWall = 0;

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

        playera = playera > 359 ? 0 : playera;
        playera = playera < 0 ? 359 : playera;
        setCameraAngle(playera);

        if (currentKey == KEY_MOVELEFT)
        {
            playerx += sina;
            playery -= cosa;

            //printf("%d, %d\n", playerx, playery);
        }
        if (currentKey == KEY_MOVERIGHT)
        {
            playerx -= sina;
            playery += cosa;

            //printf("%d, %d\n", playerx, playery);
        }

        if (currentKey == KEY_FORWARD)
        {
            //if (!(testFilled(0) < 4 && typeAtCenterOfView == TYPE_OBJECT))
            //{
            playerx += cosa;
            playery += sina;

            //}
                //printf("%d, %d\n", playerx, playery);
        }
        if (currentKey == KEY_BACK)
        {
            playerx -= cosa;
            playery -= sina;
            //printf("%d, %d\n", playerx, playery);
        }

        for (x=0;x < nScreenWidth; x+=256)
        {

            tmp = ((long)x) << 8;
            //printf("%ld\n", tmp);
            fRayAngle = (playera*256 - fFOV/2) + ((tmp/(long)nScreenWidth)*fFOV)/256;
            fDistanceToWall = 0;
            bHitWall = 0;
            while(!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall+=25;
                nTestX = playerx + (((long)cosa * (long)fDistanceToWall)/256);
                nTestY = playery + (((long)sina * (long)fDistanceToWall)/256);
                if(nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = 1;
                    fDistanceToWall = fDepth;
                }
                else
                {
                    mapIdx = ((((long)nTestY * (long)nMapWidth)/256) + nTestX)/256;
                    //printf("distance=%d\n", mapIdx);
                    if(map[mapIdx] == '#')
                    {
                        bHitWall = 1;
                        //printf("critical hit %d!\n", mapIdx);
                    }
                }
                //printf("distance=%d --- test=%d\n", fDistanceToWall/256,nTestX/256);
            }
            nCeiling = (nScreenHeight/2) - ((long)nScreenHeight*256)/fDistanceToWall;
            nFloor = nScreenHeight - nCeiling;
            //printf("%d\n", fDistanceToWall);
            for(y = 0; y < nScreenHeight; y+=256)
            {
                if(y < nCeiling)
                {
                    vpoke(0, y+x/128, ' ');
                }
                else if(y > nCeiling && y <= nFloor)
                {
                    vpoke(0, y+x/128, 0x80+' ');
                }
                else
                {
                    vpoke(0, y+x/128, ' ');
                }
            }
        }
        //printf("%2f\n", fRayAngle/256);
    }

    return 0;
}
