#ifndef _TEXTUSERINTERFACE_H_
#define _TEXTUSERINTERFACE_H_

#include <Types.hpp>
#include <string.hpp>
#include <sys/MemoryManagement.hpp>

namespace TextUserInterface {
	struct MenuEntryItem {
		char *MenuName;
		unsigned char Color;
	};

	class MenuEntry {
		public:
			void CreateMenu(int X , int Y , int Width , int Height , const char *Title);
			void AddItem(const char *ItemName , unsigned char Color);
			void RemoveItem(const char *ItemName);
			void RenameItem(const char *ItemName , const char *NewName);
			inline void Select(int ItemNumber) {
				Selected = ItemNumber;
				DrawMenu();
			}
			inline void Up(void) {
				if(Selected <= 0) {
					return;
				}
				Selected--;
				DrawMenu();
			}
			inline void Down(void) {
				if(Selected >= ItemCount-1) {
					return;
				}
				Selected++;
				DrawMenu();
			}

			int ItemCount = 0;
			int Selected;
		private:
			int X;
			int Y;
			int Width;
			int Height;
			char Title[80];
			MenuEntryItem ItemList[8192];
			void DrawMenu(void);
	};
	void CreateWindow(int X , int Y , int Width , int Height , unsigned char Color , unsigned char TitleColor , const char *Title);
};

#endif