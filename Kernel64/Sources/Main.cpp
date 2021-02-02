#include <TextScreen.hpp>
#include <string.hpp>
#include <sys/Hardware.hpp>
#include <sys/Task.hpp>
#include <sys/MemoryManagement.hpp>
#include <TextUserInterface.hpp>
/*
JuhaOS project, started in 2020.08.24
(C) 2017-2020. all rights reserved by Juha.
*/

using namespace System;
using namespace System::Memory;

void WatchDog(void);
void JumpToMainKernel(void);
static void CreateRectangle(int X , int Y , int Width , int Height , int Character , unsigned char Color);

extern "C" void Main(void) {
	int i;
	int j;
	int Color;
	char OSInfo[200];
	unsigned char UserInput;
	unsigned long *MultibootInfo = (unsigned long*)0x12000;
	unsigned long MultibootMagic = MultibootInfo[0];
	unsigned long MultibootAddress = MultibootInfo[1];

    System::Hardware::InitSystem();
    delay(500);
//    System::Task::CreateTask((unsigned long)WatchDog , TASK_DEFAULT , "WatchDog" , "The Detective Dog, Finds system error and, handle them");
    TextScreen::EnableCursor(0xFF , 0xFF);
    TextScreen::ClearScreen(0x07);
    TextScreen::MoveCursor(0 , 24);
    TextScreen::SetColor(0x70);
    for(i = 0; i < 80; i++) {
    	TextScreen::PrintCharacter(' ');
    }
    TextScreen::SetColor(0x01);
    TextScreen::MoveCursor(68 , 1);
    TextScreen::PrintString("(tm)");
    TextScreen::MoveCursor(9 , 2);
    delay(100);
    TextScreen::PrintString("    /$$$$$           /$$                  /$$$$$$   /$$$$$$ ");
    TextScreen::MoveCursor(9 , 3);
    delay(100);
    TextScreen::PrintString("   |__  $$          | $$                 /$$__  $$ /$$__  $$");
    TextScreen::MoveCursor(9 , 4);
    delay(100);
    TextScreen::PrintString("      | $$ /$$   /$$| $$$$$$$   /$$$$$$ | $$  \\ $$| $$  \\__/");
    TextScreen::MoveCursor(9 , 5);
    delay(100);
    TextScreen::PrintString("      | $$| $$  | $$| $$__  $$ |____  $$| $$  | $$|  $$$$$$ ");
    TextScreen::MoveCursor(9 , 6);
    delay(100);
    TextScreen::PrintString(" /$$  | $$| $$  | $$| $$  \\ $$  /$$$$$$$| $$  | $$ \\____  $$");
    TextScreen::MoveCursor(9 , 7);
    delay(100);
    TextScreen::PrintString("| $$  | $$| $$  | $$| $$  | $$ /$$__  $$| $$  | $$ /$$  \\ $$");
    TextScreen::MoveCursor(9 , 8);
    delay(100);
    TextScreen::PrintString("|  $$$$$$/|  $$$$$$/| $$  | $$|  $$$$$$$|  $$$$$$/|  $$$$$$/");
    TextScreen::MoveCursor(9 , 9);
    delay(100);
    TextScreen::PrintString(" \\______/  \\______/ |__/  |__/ \\_______/ \\______/  \\______/ ");

    delay(500);
    TextScreen::SetColor(0x07);
    sprintf(OSInfo , "JuhaOS Version %s" , OS_VERSION);
    TextScreen::MoveCursor(80/2-strlen(OSInfo)/2 , 11);
    TextScreen::printf("JuhaOS Version %s" , OS_VERSION);
    TextScreen::MoveCursor(0 , 24);
    TextScreen::SetColor(0x70);
    TextScreen::printf("%s" , OS_COPYRIGHT);

    TextUserInterface::MenuEntry *MainMenu;
    MainMenu->CreateMenu(80/2-((strlen(OSInfo)+16)/2) , 12 , strlen(OSInfo)+16 , 7 , "Menu");
    MainMenu->AddItem("Install JuhaOS" , 0x07);
    MainMenu->AddItem("Test JuhaOS Now" , 0x07);
    MainMenu->AddItem("JuhaOS Console" , 0x07);
    MainMenu->AddItem("Recovery Mode" , 0x07);
    MainMenu->AddItem("Reboot" , 0x07);
    TextScreen::SetColor(0x02);
	while(1) {
		UserInput = getch();
		switch(UserInput) {
			case KEYBOARD_ARROW_UP:
				MainMenu->Up();
				break;
			case KEYBOARD_ARROW_DOWN:
				MainMenu->Down();
				break;
			case '\n':
				switch(MainMenu->Selected) {
					case 0:
						break;
					case 1:
						JumpToMainKernel();
						while(1) {
							;
						}
						break;
				}
				break;
		}
 	} 	
}

static void CreateRectangle(int X , int Y , int Width , int Height , int Character , unsigned char Color) {
	int i;
	int j;
	TextScreen::MoveCursor(X , Y);
	for(i = 0; i < Width; i++) {
		TextScreen::PrintCharacter(Character);
	}
	for(j = 0; j < Height-2; j++) {
		TextScreen::MoveCursor(X , Y+j+1);
		TextScreen::PrintCharacter(Character);
		for(i = 0; i < Width-2; i++) {
			TextScreen::PrintCharacter(' ');
		}
		TextScreen::PrintCharacter(Character);
	}
	TextScreen::MoveCursor(X , Y+Height-1);
	for(i = 0; i < Width; i++) {
		TextScreen::PrintCharacter(Character);
	}
}

void JumpToMainKernel(void) {
	__asm__ ("jmp 0x300000");
}

void WatchDog(void) {
	while(1) {
		;
	}
}