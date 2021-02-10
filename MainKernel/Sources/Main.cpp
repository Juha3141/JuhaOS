#include <TextScreen.hpp>
/*
JuhaOS project, started in 2020.08.24
(C) 2017-2020. all rights reserved by Juha.
*/

extern "C" void Main(void) {
	TextScreen::ClearScreen(0x07);
    TextScreen::EnableCursor(14 , 15);
	TextScreen::printf("Hello, world!");
	while(1) {
		;
	}
}