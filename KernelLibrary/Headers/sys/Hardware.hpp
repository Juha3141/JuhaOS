#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <Types.hpp>

#define KEYBOARD_ARROW_UP 128
#define KEYBOARD_ARROW_DOWN 129
#define KEYBOARD_ARROW_RIGHT 130
#define KEYBOARD_ARROW_LEFT 131
#define KEYBOARD_ESC 132
#define KEYBOARD_F1 133
#define KEYBOARD_F2 134
#define KEYBOARD_F3 135
#define KEYBOARD_F4 136
#define KEYBOARD_F5 137
#define KEYBOARD_F6 138
#define KEYBOARD_F7 139
#define KEYBOARD_F8 140
#define KEYBOARD_F9 141
#define KEYBOARD_F10 142
#define KEYBOARD_F11 143
#define KEYBOARD_F12 144
#define KEYBOARD_CTRL 145
#define KEYBOARD_ALT 146
#define KEYBOARD_DELETE 147

#define TIMER_NANOSECOND 100
#define TIMER_CYCLE (TIMER_NANOSECOND*1193180)/1000000

namespace System {
    namespace Hardware {
        extern "C" void WritePort(unsigned short Port , unsigned char Data);
        extern "C" unsigned char ReadPort(unsigned short Port);
        void InitSystem(void);
        void EnableA20(void);
        namespace Keyboard {
            class Controller {
                public:
                    Queue ASCII;
                    Queue EXT;
                    bool Shift;
                    bool Capslock;
                    bool Numlock;
                    bool Scrolllock;
            };
            void Initialize(void);
            void ProcessAndPutToQueue(unsigned char KeyCode);
            void ProcessEXTAndPutToQueue(unsigned char KeyCode);
            bool GetKeyData(unsigned char *ASCIICode);
        };
        namespace Mouse {
            class Controller {
                public:
                    int X;
                    int Y;
                    int Button;
                    bool Status;
                    int Phase;
                    unsigned char Buffer[3];
                    Queue MouseQueue;
            };
            void Initialize(void);
            void ProcessAndPutToQueue(unsigned char KeyCode);
            bool GetMouseData(int *X , int *Y , int *Button);
        };
        namespace Timer {
            class Controller {
                public:
                    bool Initialized;
                    unsigned long TickCount;
            };
            void Initialize(void);
            void Reinitialize(unsigned short Count);
            void Interrupt(void);
        }
    }
}

unsigned long GetTickCount();
int getch(void);
void delay(unsigned long Millisecond);

#endif