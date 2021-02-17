#include <WindowSystem.hpp>
#include <sys/MemoryManagement.hpp>

void WindowSystem::Initialize(GraphicSystem Graphics) {
    this->Graphics = Graphics;
}

Window *WindowSystem::CreateWindow(int X , int Y , int Width , int Height) {
    int i = 0;
    do {
        if(Windows[i].Using == false) {
            Windows[i].Using = true;
            break;
        }
    }while(i += 1);
    Windows[i].Buffer = (unsigned short*)System::Memory::malloc(Width*Height*sizeof(unsigned short));
    Windows[i].X = X;
    Windows[i].Y = Y;
    Windows[i].Width = Width;
    Windows[i].Height = Height;
    Windows[i].ID = i;
    return &(Windows[i]);
}