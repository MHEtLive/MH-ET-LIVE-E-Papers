/**
 *  @filename   :   main.cpp
 *  @brief      :   1.54inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "epd1in54.h"
#include "epdpaint.h"
#include "imagedata.h"

#define COLORED      0
#define UNCOLORED    1

int main(void)
{
    time_t now;
    struct tm* timenow;

    Epd epd;
    if (epd.Init(lut_full_update) != 0) {
        printf("e-Paper init failed\n");
        return -1;
    }

    unsigned char* frame_buffer = (unsigned char*)malloc(epd.width / 8 * epd.height);

    Paint paint(frame_buffer, epd.width, epd.height);
    paint.Clear(UNCOLORED);

    /* For simplicity, the arguments are explicit numerical coordinates */
    /* Write strings to the buffer */
    paint.DrawFilledRectangle(0, 10, 200, 34, COLORED);
    paint.DrawStringAt(30, 14, "Hello world!", &Font16, UNCOLORED);
    paint.DrawStringAt(30, 34, "e-Paper Demo", &Font16, COLORED);

    /* Draw something to the frame_buffer */
    paint.DrawRectangle(16, 60, 56, 110, COLORED);
    paint.DrawLine(16, 60, 56, 110, COLORED);
    paint.DrawLine(56, 60, 16, 110, COLORED);
    paint.DrawCircle(120, 90, 30, COLORED);
    paint.DrawFilledRectangle(16, 130, 56, 180, COLORED);
    paint.DrawFilledCircle(120, 160, 30, COLORED);

    /* Display the frame_buffer */
    epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();
    epd.DelayMs(2000);

    if (epd.Init(lut_partial_update) != 0) {
        printf("e-Paper init failed\n");
        return -1;
    }

  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
    epd.SetFrameMemory(IMAGE_DATA, 0, 0, epd.width, epd.height);
    epd.DisplayFrame();
    epd.SetFrameMemory(IMAGE_DATA, 0, 0, epd.width, epd.height);
    epd.DisplayFrame();

    char time_string[] = {'0', '0', ':', '0', '0', '\0'};
    while (1) {
        time(&now);
        timenow = localtime(&now);
        time_string[0] = timenow->tm_min / 10 + '0';
        time_string[1] = timenow->tm_min % 10 + '0';
        time_string[3] = timenow->tm_sec / 10 + '0';
        time_string[4] = timenow->tm_sec % 10 + '0';

        paint.SetWidth(32);
        paint.SetHeight(96);
        paint.SetRotate(ROTATE_270);
        paint.Clear(UNCOLORED);
        paint.DrawStringAt(0, 4, time_string, &Font24, COLORED);

        epd.SetFrameMemory(paint.GetImage(), 80, 72, paint.GetWidth(), paint.GetHeight());
        epd.DisplayFrame();
        epd.DelayMs(500);
    }
    return 0;
}

