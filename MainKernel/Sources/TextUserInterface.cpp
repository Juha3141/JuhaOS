#include <TextUserInterface.hpp>
#include <TextScreen.hpp>
#include <string.hpp>

void TextUserInterface::CreateWindow(int X , int Y , int Width , int Height , unsigned char Color , unsigned char TitleColor , const char *Title) {
	int i;
	int j;
	int BackupColor = TextScreen::GetColor();
	int OffsetX = X;
	int OffsetY = Y+1;
	TextScreen::MoveCursor(X , Y);
	TextScreen::SetColor(Color);
	if(Width < strlen(Title)+1) {
		return;
	}
	TextScreen::PrintCharacter(201);
	TextScreen::PrintCharacter(181);
	TextScreen::SetColor(TitleColor);
	TextScreen::printf("%s" , Title);
	TextScreen::SetColor(Color);
	TextScreen::PrintCharacter(198);
	for(i = 0; i < Width-4-strlen(Title); i++) {
		TextScreen::PrintCharacter(205);
	}
	TextScreen::PrintCharacter(187);
	for(i = 0; i < Height-2; i++) {
		TextScreen::MoveCursor(OffsetX , OffsetY);
		TextScreen::PrintCharacter(186);
		for(j = 0; j < Width-2; j++) {
			TextScreen::printf(" ");
		}
		TextScreen::PrintCharacter(186);
		OffsetY += 1;
	}
	TextScreen::MoveCursor(OffsetX , OffsetY);
	TextScreen::PrintCharacter(200);
	for(i = 0; i < Width-2; i++) {
		TextScreen::PrintCharacter(205);
	}
	TextScreen::PrintCharacter(188);
}

void TextUserInterface::MenuEntry::CreateMenu(int X , int Y , int Width , int Height , const char *Title) {
	this->X = X;
	this->Y = Y;
	this->Width = Width;
	this->Height = Height;
	strcpy(this->Title , Title);
	this->ItemCount = 0;
	this->Selected = 0;
	this->DrawMenu();
}

void TextUserInterface::MenuEntry::AddItem(const char *ItemName , unsigned char Color) {
	ItemList[ItemCount].MenuName = (char*)System::Memory::malloc(strlen(ItemName));
	strcpy(ItemList[ItemCount].MenuName , ItemName);
	ItemList[ItemCount].Color = Color;
	ItemCount += 1;
	DrawMenu();
}

void TextUserInterface::MenuEntry::DrawMenu(void) {
	int i;
	int j;
	int Offset;
	unsigned char BackgroundColor;
	unsigned char TextColor;
	TextUserInterface::CreateWindow(X , Y , Width , Height , 0x07 , 0x0E , Title);
	for(i = 0; i < ItemCount; i++) {
		Offset = ((Width-2)/2)-(strlen(ItemList[i].MenuName)/2);
		BackgroundColor = ItemList[i].Color >> 4;
		if(BackgroundColor == 0) {
			TextColor = ItemList[i].Color;
		}
		else {
			TextColor = (ItemList[i].Color >> 4) & ItemList[i].Color;
		}
		if(i == Selected) {
			TextScreen::SetColor((TextColor << 4)+BackgroundColor);
		}
		else {
			TextScreen::SetColor(ItemList[i].Color);
		}
		TextScreen::MoveCursor(X+1 , Y+1+i);
		for(j = 0; j < Width-2; j++) {
			TextScreen::printf(" ");
		}
		TextScreen::MoveCursor(X+1+Offset , Y+1+i);
		TextScreen::printf("%s" , ItemList[i].MenuName);
	}
}