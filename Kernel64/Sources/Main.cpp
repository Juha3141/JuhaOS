#include <TextScreen.hpp>
#include <string.hpp>
#include <sys/Hardware.hpp>
#include <sys/Task.hpp>
#include <sys/MemoryManagement.hpp>
#include <TextUserInterface.hpp>

#include <Graphics.hpp>
#include <WindowSystem.hpp>

extern "C" void Main(void) {
    System::Hardware::InitSystem();
    int X = 100;
    int Y = 100;

    int DeltaX;
    int DeltaY;
    int Button;
    VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)0x1000;
    GraphicSystem System((unsigned short*)(Block->Address) , Block->Width , Block->Height);

    System.DrawRectangle(X , Y , 9 , 9 , 0x00);
    System.DrawRectangle(X+2 , Y+2 , 5 , 5 , RGB(0xFF , 0xFF , 0xFF));
    while(1) {
        if(System::Hardware::Mouse::GetMouseData(&(DeltaX) , &(DeltaY) , &(Button)) == false) {
            continue;
        }
        X += DeltaX;
        Y += DeltaY;

        System.DrawRectangle(X , Y , 9 , 9 , 0x00);
        System.DrawRectangle(X+2 , Y+2 , 5 , 5 , RGB(0xFF , 0xFF , 0xFF));
    }
}