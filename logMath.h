#ifndef LOGMATH_H
#define LOGMATH_H

#include <cc65.h>

static int angle;
static int sina;
static int cosa;
static int cameraX;
static int cameraY;
void __fastcall__ setCameraAngle(int a)
{
    angle = a;
    sina = _sin(a);
    cosa = _cos(a);
    /*__asm__("sta %v", angle);
    __asm__("tay");
    __asm__("lda sintab,y");
    __asm__("sta %v", sina);
    __asm__("tya");
    __asm__("clc");
    __asm__("adc #16");
    __asm__("and #63");
    __asm__("tay");
    __asm__("lda sintab,y");
    __asm__("sta %v", cosa);*/
}

void __fastcall__ setCameraX(int val)
{
    cameraX = val;
}

void __fastcall__ setCameraY(int val)
{
    cameraY = val;
}

void line(int x0, int y0, int x1, int y1, uint8_t v) {

  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    if(x0 >=0 && y0>=0 && x0 < 128 && y0 < 128)
        vpoke(0, (y0*128+x0)*2, v);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}


#endif // LOGMATH_H
