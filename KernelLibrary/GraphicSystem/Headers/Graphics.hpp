#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <Types.hpp>
#include <TextScreen.hpp>

#define RGB(Red , Green , Blue) (((unsigned char)((Red) >> 3) << 11)|((unsigned char)((Green) >> 2) << 5)|((unsigned char)((Blue) >> 3)))

class GraphicSystem {
    public:
        GraphicSystem(unsigned short *VideoMemoryAddress , unsigned short Width , unsigned short Height);
        void DrawPixel(int X , int Y , unsigned short Color);
        void DrawLine(int X1 , int Y1 , int X2 , int Y2 , unsigned short Color);
        void DrawRectangle(int X1 , int Y1 , int Width , int Height , unsigned short Color);
        void DrawText(const char *Text , int X , int Y , unsigned short Color);
    private:
        unsigned short *VideoMemory;
        unsigned short Width;
        unsigned short Height;
};

#endif