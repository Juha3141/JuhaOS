#include <TextScreen.hpp>
#include <string.hpp>
#include <sys/Hardware.hpp>

using namespace System;

static TextScreen::TextScreenController ScreenController;

void TextScreen::EnableCursor(unsigned char Start , unsigned char End) {
    Hardware::WritePort(0x3D4 , 0x0A);
    Hardware::WritePort(0x3D5 , (Hardware::ReadPort(0x3D5) & 0xC0)|Start);
    Hardware::WritePort(0x3D4 , 0x0B);
    Hardware::WritePort(0x3D5 , (Hardware::ReadPort(0x3D5) & 0xE0)|End);
}

void TextScreen::ClearScreen(char Color) {
    int i;
    unsigned char *Buffer = (unsigned char*)TEXTSCREEN_BUFFER;
    ScreenController.Color = Color;
    for(i = 0; i < TEXTSCREEN_WIDTH*TEXTSCREEN_HEIGHT; i++) {
        *(Buffer++) = 0x00;
        *(Buffer++) = Color;
    }
    MoveCursor(0 , 0);
    return;
}

void TextScreen::MoveCursor(int X , int Y) {
    int Offset = (Y*TEXTSCREEN_WIDTH)+X;
    Hardware::WritePort(0x3D4 , 0x0E);
    Hardware::WritePort(0x3D5 , Offset >> 8);
    Hardware::WritePort(0x3D4 , 0x0F);
    Hardware::WritePort(0x3D5 , Offset & 0xFF);
    ScreenController.X = X;
    ScreenController.Y = Y;
}

void TextScreen::GetCursor(int *X , int *Y) {
    *X = ScreenController.X;
    *Y = ScreenController.Y;
}

void TextScreen::MoveCursorWithoutSaving(int X , int Y) {
    int Offset = (Y*TEXTSCREEN_WIDTH)+X;
    Hardware::WritePort(0x3D4 , 0x0E);
    Hardware::WritePort(0x3D5 , Offset >> 8);
    Hardware::WritePort(0x3D4 , 0x0F);
    Hardware::WritePort(0x3D5 , Offset & 0xFF);
}

void TextScreen::PrintCharacter(const unsigned char Character) {
    int i;
    int Offset = ((ScreenController.Y*TEXTSCREEN_WIDTH)+ScreenController.X)*2;
    unsigned char *Buffer = (unsigned char*)TEXTSCREEN_BUFFER+Offset;
    switch(Character) {
        case '\n':
            ScreenController.X = 0;
            ScreenController.Y += 1;
            break;
        case '\r':
            ScreenController.X = 0;
            break;
        case '\b':
            if(ScreenController.X > 0) {
                ScreenController.X -= 1;
                Buffer = (unsigned char*)TEXTSCREEN_BUFFER;
                Buffer[(((ScreenController.Y*TEXTSCREEN_WIDTH)+(ScreenController.X))*2)] = 0x00;
                Buffer[(((ScreenController.Y*TEXTSCREEN_WIDTH)+(ScreenController.X))*2)+1] = ScreenController.Color;
                Buffer += ((ScreenController.Y*TEXTSCREEN_WIDTH)+ScreenController.X)*2;
            }
            break;
        case '\t':
            for(i = 0; i < 5; i++) {
                *Buffer++ = ' ';
                ScreenController.X++;
            }
            break;
        default:
            *Buffer++ = Character;
            *Buffer++ = ScreenController.Color;
            ScreenController.X++;
            break;
    }
    if(ScreenController.X > TEXTSCREEN_WIDTH) {
        ScreenController.X = 0;
        ScreenController.Y += 1;
    }
    Buffer = (unsigned char*)TEXTSCREEN_BUFFER;
    if(ScreenController.Y >= TEXTSCREEN_HEIGHT) {
        for(i = 0; i < TEXTSCREEN_WIDTH*(TEXTSCREEN_HEIGHT-1)*2; i++) {
            Buffer[i] = Buffer[i+(80*2)];
        }
        for(i = TEXTSCREEN_WIDTH*(TEXTSCREEN_HEIGHT-1)*2; i < TEXTSCREEN_WIDTH*(TEXTSCREEN_HEIGHT)*2;) {
            Buffer[i++] = ' ';
            Buffer[i++] = ScreenController.Color;
        }
        ScreenController.X = 0;
        ScreenController.Y = 24;
    }
    MoveCursorWithoutSaving(ScreenController.X , ScreenController.Y);
}

void TextScreen::PrintString(const char *Buffer) {
    int i;
    for(i = 0; Buffer[i] != '\0'; i++) {
        PrintCharacter(Buffer[i]);
    }
}

void TextScreen::printf(const char *Format , ...) {
    va_list ap;
    char Buffer[0x4000];
    va_start(ap , Format);
    vsprintf(Buffer , Format , ap);
    PrintString(Buffer);
    va_end(ap);
}

void TextScreen::SetColor(char Color) {
    ScreenController.Color = Color;
}

char TextScreen::GetColor(void) {
    return ScreenController.Color;
}