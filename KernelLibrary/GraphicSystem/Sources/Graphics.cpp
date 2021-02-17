#include <Graphics.hpp>
#include <WindowSystem.hpp>
#include <sys/MemoryManagement.hpp>
#include <Font.hpp>
#include <string.hpp>

GraphicSystem::GraphicSystem(unsigned short *VideoMemoryAddress , unsigned short Width , unsigned short Height) {
    this->VideoMemory = VideoMemoryAddress;
    this->Width = Width;
    this->Height = Height;
}

void GraphicSystem::DrawPixel(int X , int Y , unsigned short Color) {
    if(X < 0) {
        X = 0;
    }
    if(Y < 0) {
        Y = 0;
    }
    if(X > Width) {
        X = Width;
    }
    if(Y > Height) {
        Y = Height;
    }
    this->VideoMemory[(Y*Width)+X] = Color;
}

void GraphicSystem::DrawLine(int X1 , int Y1 , int X2 , int Y2 , unsigned short Color) {
    int i;
    int X = 0;
    int Y = 0;
    int DX = X2-X1;
    int DY = Y2-Y1;
    int IX = (DX < 0) ? -DX : DX;
    int IY = (DY < 0) ? -DY : DY;
    int Delta = (IX > IY) ? IX : IY;
    int PointX = X1;
    int PointY = Y1;
    for(i = 0; i <= Delta; i++) {
        X += IX;
        Y += IY;
        if(X >= Delta) {
            X -= Delta;
            if(DX > 0) {
                PointX += 1;
            }
            else {
                if(DX != 0) {
                    PointX += -1;
                }
            }
        }
        if(Y >= Delta) {
            Y -= Delta;
            if(DY > 0) {
                PointY += 1;
            }
            else {
                if(DY != 0) {
                    PointY += -1;
                }
            }
        }
        DrawPixel(PointX , PointY , Color);
    }
}

void GraphicSystem::DrawRectangle(int X1 , int Y1 , int Width , int Height , unsigned short Color) {
    int X;
    int Y;
    for(Y = 0; Y < Height; Y++) {
        for(X = 0; X < Width; X++) {
            this->DrawPixel(X+X1 , Y+Y1 , Color);
        }
    }
}

void GraphicSystem::DrawText(const char *Text , int X , int Y , unsigned short Color) {
    int i;
    int j;
    int k;
    int CurrentX;
    int CurrentY;
    unsigned char BitMask;
    int BitMaskStartAddress;
    unsigned short *VideoMemory = this->VideoMemory;
    const int Width = 8;
    const int Height = 16;
    int HeightBackup;
    CurrentX = X;
    for(k = 0; k < strlen(Text); k++) {
        CurrentY = Y;
        BitMaskStartAddress = Text[k]*Height;
        for(j = 0; j < Height; j++) {
            BitMask = Consolas_8x16_Bold[BitMaskStartAddress++];
            for(i = 0; i < Width; i++) {
                if(BitMask & (0x01 << (Width-i-1))) {
                    DrawPixel(CurrentX+i , CurrentY , Color);
                }
            }
            CurrentY += 1;
        }
        CurrentX += Width;
    }
}