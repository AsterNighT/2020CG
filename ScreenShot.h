#ifndef SCREENSHOT_H
#define SCREENSHOT_H

typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long           LONG;

#include <Windows.h>

bool ScreenShot(int height, int width, const char* filename) ;

#endif
