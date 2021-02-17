#ifndef _WINDOWSYSTEM_H_
#define _WINDOWSYSTEM_H_

#include <Types.hpp>
#include <Graphics.hpp>
#include <sys/Task.hpp>

#define WINDOW_MAXCOUNT 512

class Window {
    friend class WindowSystem;
    public:
        unsigned long ID;
        unsigned long TaskID;
    private:
        bool Using = false;
        int X;
        int Y;
        int Width;
        int Height;
        unsigned short *Buffer;
};

class WindowSystem {
    friend class Window;
    public:
        void Initialize(GraphicSystem Graphics);
        Window *CreateWindow(int X , int Y , int Width , int Height);

    protected:
        Window Windows[WINDOW_MAXCOUNT];
        GraphicSystem Graphics;
};

#endif