// Cursor.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <csignal>
#include<math.h>
#include <ShellScalingApi.h>
#include <winuser.h>




using namespace std;





void cursorposition(int &x_coord, int &y_coord)
{
    POINT cursor;
    if (GetCursorPos(&cursor))
    {
        x_coord = cursor.x;
        y_coord = cursor.y;
    } 
}
/*
void scalefac(int x, int y, float &scale)
{
    POINT pt;
    pt.x = x;
    pt.y = y;
    HMONITOR monitor = MonitorFromPoint( pt, MONITOR_DEFAULTTONEAREST);
    DEVICE_SCALE_FACTOR pScale;
    GetScaleFactorForMonitor( monitor, &pScale);
    
    switch(pScale) 
        {
    SCALE_100_PERCENT:
        scale = 1.0;
        break;
    SCALE_120_PERCENT:
        scale = 1.20;
        break;
    SCALE_125_PERCENT:
        scale = 1.25;
        break;
    SCALE_140_PERCENT:
        scale = 1.40;
        break;
    SCALE_150_PERCENT:
        scale = 1.50;
        break;
    SCALE_160_PERCENT:
        scale = 1.60;
        break;
    SCALE_175_PERCENT:
        scale = 1.75;
        break;
    SCALE_180_PERCENT:
        scale = 1.80;
        break;
    SCALE_200_PERCENT:
        scale= 2.0;
        break;
    SCALE_225_PERCENT:
        scale = 2.25;
        break;
    SCALE_250_PERCENT:
        scale = 2.50;
        break;
    SCALE_300_PERCENT:
        scale = 3.0;
        break;
    SCALE_350_PERCENT:
        scale = 3.50;
        break;
    SCALE_400_PERCENT:
        scale = 4.0;
        break;
    SCALE_450_PERCENT:
        scale = 4.50;
        break;
    SCALE_500_PERCENT:
        scale = 5.0;
        break;
    }

   
}
*/


void getcolour(int x_coord, int y_coord, int &r, int&g, int&b)
{
    
    COLORREF color;
    HDC hDC;
    

    // Get the device context for the screen
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return;

    // Get the current cursor position
    

    // Retrieve the color at that position
    color = GetPixel(hDC, x_coord, y_coord);
    if (color == CLR_INVALID)
        return;

    // Release the device context again
    ReleaseDC(GetDesktopWindow(), hDC);

    r = GetRValue(color);
    g = GetGValue(color); 
    b = GetBValue(color);
}

void draw_circle(int centre_x, int centre_y, int radius, COLORREF color, COLORREF oppositecolor, HDC hdc)
{
    int offset_y = 0;
    int offset_x = radius;
    int crit = 1 - radius;
    while (offset_y <= offset_x)
    {
        SetPixel(hdc, centre_x + offset_x, centre_y + offset_y, color); // octant 1
        SetPixel(hdc, centre_x + offset_y, centre_y + offset_x, color); // octant 2
        SetPixel(hdc, centre_x - offset_x, centre_y + offset_y, color); // octant 4
        SetPixel(hdc, centre_x - offset_y, centre_y + offset_x, color); // octant 3
        SetPixel(hdc, centre_x - offset_x, centre_y - offset_y, color); // octant 5
        SetPixel(hdc, centre_x - offset_y, centre_y - offset_x, color); // octant 6
        SetPixel(hdc, centre_x + offset_x, centre_y - offset_y, color); // octant 7
        SetPixel(hdc, centre_x + offset_y, centre_y - offset_x, color); // octant 8

        offset_y = offset_y + 1;
        if (crit <= 0)
        {
            crit = crit + 2 * offset_y + 1;
        }
        else
        {
            offset_x = offset_x - 1;
            crit = crit + 2 * (offset_y - offset_x) + 1;
        }
    }
}


void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

void erase(int x_coord, int y_coord, int r, int g, int b)
{
    COLORREF color, oppositecolor;
    HDC hDC;

    // Get the device context for the screen
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return;

    int horizontal, vertical;
    GetDesktopResolution(horizontal, vertical);
    int size = pow((pow(horizontal, 2) + pow(vertical, 2)), 0.5);

    oppositecolor = RGB(255 - r, 255 - g, 255 - b);
    color = RGB(255 - r, 255 - g, 255 - b);

    for (int radius = size / 20; radius >= 5; radius = radius - 5)


    {
        draw_circle(x_coord, y_coord, radius, color, oppositecolor, hDC);
    }

    ReleaseDC(GetDesktopWindow(), hDC);
    RECT rect;
    rect.left = x_coord - 1000;
    rect.right = x_coord + 1000;
    rect.top = y_coord - 1000;
    rect.bottom = y_coord + 1000;
    HWND screen = GetForegroundWindow();
    HWND desktopscreen = GetDesktopWindow();

    InvalidateRect(screen, NULL, true);
    RedrawWindow(screen, &rect, NULL, RDW_ERASE);
    InvalidateRect(desktopscreen, NULL, true);
    RedrawWindow(desktopscreen, &rect, NULL, RDW_ERASE);
}


void locate(int x_coord, int y_coord, int r, int g, int b)
{
    COLORREF color, oppositecolor;
    HDC hDC;

    // Get the device context for the screen
    hDC = GetDC(NULL);
    if (hDC == NULL)
        return;

    int horizontal, vertical;
    GetDesktopResolution(horizontal, vertical);
    int size = pow((pow(horizontal, 2) + pow(vertical, 2)), 0.5);

    oppositecolor = RGB(255 - r, 255 - g, 255 - b);
    color = RGB(255 - r, 255 - g, 255 - b);

    for (int radius = 10; radius <= size/20; radius += 10)
    {
        for (int r = radius; r < radius + 3; r += 1)
        {
            draw_circle(x_coord, y_coord, r, color, oppositecolor, hDC);
        }
    }

    ReleaseDC(GetDesktopWindow(), hDC);
    RECT rect;
    rect.left = x_coord - 1000;
    rect.right = x_coord + 1000;
    rect.top = y_coord - 1000;
    rect.bottom = y_coord + 1000;
    HWND screen = GetForegroundWindow();
    HWND desktopscreen = GetDesktopWindow();
    InvalidateRect(screen, NULL, true);
    RedrawWindow(screen, &rect, NULL, RDW_ERASE);
    InvalidateRect(desktopscreen, NULL, true);
    RedrawWindow(desktopscreen, &rect, NULL, RDW_ERASE);
    
}




int main()

{
    int x_coord, y_coord, r, g, b;
    int horizontalscreen, verticalscreen;
    float scale;
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);


        

        while(true) {

         if ((GetKeyState(VK_CONTROL) &(GetKeyState(VK_LWIN)&0x8000)))
        {
             GetDesktopResolution(horizontalscreen, verticalscreen);
            SetCursorPos(horizontalscreen / 2, verticalscreen / 2);
        }
         
        else if (GetKeyState(VK_CONTROL) & 0x8000)
            {
            
            cursorposition(x_coord, y_coord);
         // scalefac(x_coord, y_coord, scale);
         // x_coord = x_coord * scale;
         // y_coord = y_coord * scale;

            getcolour(x_coord, y_coord, r, g, b);
            locate(x_coord, y_coord, r, g, b);
            //erase 
      erase(x_coord, y_coord, 255-r, 255-g, 255-b);
            
            cout << "x:" << x_coord << " y:" << y_coord << "\n";
            cout << "R:" << r << " G:" << g << " B:" << b << "\n \n";
              
        }
       
        else
            continue;
    }
}


