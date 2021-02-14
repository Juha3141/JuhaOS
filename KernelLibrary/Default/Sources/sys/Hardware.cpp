#include <sys/Hardware.hpp>
#include <TextScreen.hpp>
#include <sys/DescriptorTables.hpp>
#include <sys/MemoryManagement.hpp>
#include <sys/Task.hpp>

bool CheckInputBuffer(void);
bool CheckOutputBuffer(void);

using namespace System;

static Hardware::Keyboard::Controller KeyboardController;
static Hardware::Mouse::Controller MouseController;
static Hardware::Timer::Controller TimerController;

void Hardware::InitSystem(void) {
    __asm__ ("cli");
    TimerController.Initialized = false;
    Hardware::EnableA20();
    TextScreen::ClearScreen(0x07);
    TextScreen::printf("DescriptorTables  : ");
    DescriptorTables::Initialize();
    TextScreen::printf("ok\n");
    TextScreen::printf("Keyboard System   : ");
    Hardware::Keyboard::Initialize();
    Hardware::Mouse::Initialize();
    TextScreen::printf("ok\n");
    TextScreen::printf("Memory Management : ");
    Memory::Initialize();
    TextScreen::printf("Task System       : ");
    Task::Initialize();
    TextScreen::printf("ok\n");
    TextScreen::printf("System Timer      : ");
    Hardware::Timer::Initialize();
    TextScreen::printf("ok\n");
    __asm__ ("sti");
}

void Hardware::FixSystem(void) {
    __asm__ ("cli");
    TimerController.Initialized = false;
    DescriptorTables::Initialize();
    Hardware::Keyboard::Initialize();
    Memory::Initialize();
    Hardware::Timer::Initialize();
    __asm__ ("sti");
}

bool CheckOutputBuffer(void) {
    if(Hardware::ReadPort(0x64) & 0x01) {
        return false;
    }
    return true;
}

bool CheckInputBuffer(void) {
    if(Hardware::ReadPort(0x64) & 0x02) {
        return false;
    }
    return true;
}

void Hardware::EnableA20(void) {
    int i;
    unsigned char Data;
    Hardware::WritePort(0x64 , 0xD0);
    for(i = 0; i < 0xFFFF; i++) {
        if(CheckOutputBuffer() == false) {
            break;
        }
    }
    Data = Hardware::ReadPort(0x60);
    Data |= 0x01;
    for(i = 0; i < 0xFFFF; i++) {
        if(CheckInputBuffer() == true) {
            break;
        }
    }
    WritePort(0x64 , 0xD1);
    WritePort(0x60 , Data);
}

static inline void WaitForKeyboardData(void) {
    int i;
    for(i = 0; i < 0xFFFF; i++) {
        if(!(Hardware::ReadPort(0x64) & 0x02)) {
            break;
        }
    }
}

void Hardware::Keyboard::Initialize(void) {
    KeyboardController.Capslock = false;
    KeyboardController.Shift = false;
    KeyboardController.ASCII.Initialize(1024);
}

bool Hardware::Keyboard::GetKeyData(unsigned char *ASCIICode) {
    if(KeyboardController.ASCII.CheckEmpty() == true) {
        return false;
    }
    *ASCIICode = KeyboardController.ASCII.Dequeue();
    return true;
}

void Hardware::Mouse::Initialize(void) {
    unsigned char Data;
    __asm__ ("cli");
    MouseController.MouseQueue.Initialize(512);
    Hardware::WritePort(0x64 , 0xA8);
    Hardware::WritePort(0x64 , 0xD4);
    WaitForKeyboardData();
    Hardware::WritePort(0x60 , 0xF4);
    Hardware::WritePort(0x64 , 0x20);
    WaitForKeyboardData();
    Data = Hardware::ReadPort(0x60);
    Data |= 0x02;
    Hardware::WritePort(0x64 , 0x60);
    WaitForKeyboardData();
    Hardware::WritePort(0x60 , Data);
    __asm__ ("sti");
    MouseController.Phase = 0;
}

void Hardware::Timer::Initialize(void) {
    TimerController.Initialized = true;
    TimerController.TickCount = 0;
    __asm__ ("cli");
    Hardware::WritePort(0x43 , 0x30);
    Hardware::WritePort(0x43 , 0x34);
    Hardware::WritePort(0x40 , TIMER_CYCLE);
    Hardware::WritePort(0x40 , TIMER_CYCLE >> 0x08);
    __asm__ ("sti");
}

void Hardware::Timer::Reinitialize(unsigned short Count) {
    __asm__ ("cli");
    Hardware::WritePort(0x43 , 0x30);
    Hardware::WritePort(0x43 , 0x34);
    Hardware::WritePort(0x40 , Count);
    Hardware::WritePort(0x40 , Count >> 0x08);
    __asm__ ("sti");
}

unsigned long GetTickCount(void) {
    return TimerController.TickCount;
}

void Hardware::Timer::Interrupt(void) {
    TimerController.TickCount++;
}

void delay(unsigned long Millisecond) {
    unsigned long LastTick = GetTickCount();
    if(TimerController.Initialized == false) {
        return;
    }
    while(1) {
        if(GetTickCount()-LastTick >= Millisecond*TIMER_NANOSECOND/10) {
            break;
        }
    }
}

void Hardware::Mouse::ProcessAndPutToQueue(unsigned char Code) {
    MouseController.MouseQueue.Enqueue(Code);
}

bool Hardware::Mouse::GetMouseData(int *X , int *Y , int *Button) {
    unsigned char Code = MouseController.MouseQueue.Dequeue();
    switch(MouseController.Phase) {
        case 0:
            if(Code == 0xFA) {
                MouseController.Phase = 1;
            }
            return false; 
        case 1:
            if((Code & 0xC8) == 0x08) {
                MouseController.Buffer[0] = Code;
                MouseController.Phase = 2;
            }
            return false; 
        case 2:
            MouseController.Buffer[1] = Code;
            MouseController.Phase = 3;
            return false;
        case 3:
            MouseController.Buffer[2] = Code;
            MouseController.Phase = 1;
            *Button = MouseController.Buffer[0] & 0x07;
            *X = MouseController.Buffer[1];
            *Y = MouseController.Buffer[2];
            if((MouseController.Buffer[0] & 0x10) != 0x00) {
                *X |= 0xFFFFFF00;
            }
            if((MouseController.Buffer[0] & 0x20) != 0x00) {
                *Y |= 0xFFFFFF00;
            }
            *Y = -(*Y);
            return true;
        default:
            return false;
    }
    return false;
}

void Hardware::Keyboard::ProcessAndPutToQueue(unsigned char KeyCode) {
    unsigned char KeyCodeLow;
	static const unsigned char Default[0xFF] = {
		0 , 0 , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , '\b' , 0 , 
		'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '[' , ']' , '\n' , 0 , 'a' , 's' , 
		'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l', ';', '\'', '`',   0,   '\\', 'z', 'x', 'c', 'v',
		'b' , 'n' , 'm' , ',' , '.' , '/' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
        0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	};
	static const unsigned char Shift[0xFF] = {
		0 , 0 , '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , '\b' , 0 , 
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '{' , '}' , '\n' , 0 , 'A' , 'S' , 
		'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , ':' , '"' , '~' ,   0,   '|', 'Z', 'X' , 'C' , 'V' , 
		'B' , 'N' , 'M' , '<' , '>' , '?' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
        0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	};
	static const unsigned char Capslock[0xFF] = {
		0 , 0 , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , '\b' , 0 , 
		'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '[' , ']' , '\n' , 0 , 'A' , 'S' , 
		'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L', ';', '\'', '`',   0,   '\\', 'Z', 'X', 'C', 'V',
		'B' , 'N' , 'M' , ',' , '.' , '/' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
        0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	};
	static const unsigned char CapslockShift[0xFF] = {
		0 , 0 , '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , '\b' , 0 , 
		'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '{' , '}' , '\n' , 0 , 'a' , 's' , 
		'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l', ':', '"', '~',   0,   '|', 'z', 'x', 'c', 'v',
		'b' , 'n' , 'm' , '<' , '>' , '?' , 0 , '*' , 0 , ' ' , 0 , 0, 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , KEYBOARD_ARROW_UP , 0 , 0 , KEYBOARD_ARROW_LEFT , 0 , KEYBOARD_ARROW_RIGHT , 0 , 
        0 , KEYBOARD_ARROW_DOWN , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	};
    if(KeyCode & 0x80) {
        KeyCodeLow = KeyCode & 0x7F;
        if((KeyCodeLow == 42)||(KeyCodeLow == 54)) {
            KeyboardController.Shift = false;
        }
    }
    else {
        if((KeyCode == 42)||(KeyCode == 54)) {
            KeyboardController.Shift = true;
        }
        else if(KeyCode == 58) {
            if(KeyboardController.Capslock == true) {
                KeyboardController.Capslock = false;
            }
            else {
                KeyboardController.Capslock = true;
            }
        }
        else if(KeyCode == 69) {
            if(KeyboardController.Numlock == true) {
                KeyboardController.Numlock = false;
            }
            else {
                KeyboardController.Numlock = true;
            }
        }
        else if(KeyCode == 69) {
            if(KeyboardController.Scrolllock == true) {
                KeyboardController.Scrolllock = false;
            }
            else {
                KeyboardController.Scrolllock = true;
            }
        }
        if(KeyboardController.Shift == true) {
            if(KeyboardController.Capslock == true) {
                KeyboardController.ASCII.Enqueue(CapslockShift[KeyCode]);
            }
            else {
                KeyboardController.ASCII.Enqueue(Shift[KeyCode]);
            }
        }
        else if(KeyboardController.Capslock == true) {
            KeyboardController.ASCII.Enqueue(Capslock[KeyCode]);
        }
        else {
            KeyboardController.ASCII.Enqueue(Default[KeyCode]);
        }
    }
}

int getch(void) {
	int ASCIICode;
	while(1) {
		if(KeyboardController.ASCII.CheckEmpty() == false) {
            ASCIICode = KeyboardController.ASCII.Dequeue();
            if(ASCIICode != 0) {
                break;
            }
        }
	}
	return ASCIICode;
}

void Queue::Initialize(int MaxSize) {
	if(MaxSize > QUEUE_MAXSIZE) {
		MaxSize = QUEUE_MAXSIZE;
	}
	this->MaxSize = MaxSize;
	this->Rear = 0;
	this->Front = 0;
}

bool Queue::CheckEmpty(void) {
	if(Rear == Front) {
		return true;
    }
	return false;
}

bool Queue::CheckFull(void) {
	if((Rear+1)%MaxSize == Front) {
		return true;
	}
	return false;
}

bool Queue::Enqueue(unsigned char Data) {
	if(this->CheckFull() == true) {
		return false;
	}
	Rear = (Rear+1)%MaxSize;
	Buffer[Rear] = Data;
	return true;
}

unsigned char Queue::Dequeue(void) {
	if(this->CheckEmpty() == true) {
		return 0;
	}
	Front = (Front+1)%MaxSize;
	return Buffer[Front];
}