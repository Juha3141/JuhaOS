#include <string.hpp>

int SizeOfNumber(int Number);
int IndexOfNumber(int Number , int Index);

unsigned int memset(void *Destination , int Data , unsigned int Size) {
	unsigned int i;
	for(i = 0; i < Size; i++) {
		((char*)Destination)[i] = Data;
	}
	return Size;
}

unsigned int memcpy(void *Destination , const void *Source , unsigned int Size) {
    int i;
    for(i = 0; i < Size; i++) {
        ((char*)Destination)[i] = ((char*)Source)[i];
    }
	return Size;
}

int strlen(const char *Destination) {
    int i;
	for(i = 0; Destination[i] != '\0'; i++) {
		;
	}
	return i;
}

char *strcpy(char *Destination , const char *Source) {
	unsigned int i;
	memset(Destination , 0 , sizeof(Destination));
	for(i = 0; (Destination[i] != '\0')||(Source[i] != '\0'); i++) {
		Destination[i] = Source[i];
	}
	return Destination;
}

char *strcat(char *Destination , const char *Source) {
	unsigned int i = 0;
	unsigned int j = 0;
	while(Destination[i] != '\0') {
		i++;
	}
	while(Source[j] != '\0') {
		Destination[i++] = Source[j++];
	}
	Destination[i] = '\0';
	return Destination;
}

int strcmp(const char *FirstString , const char *SecondString) {
	unsigned int i;
	int NotEqual = 0;
	for(i = 0; (FirstString[i] != '\0')||(SecondString[i] != '\0'); i++) {
		if(FirstString[i] != SecondString[i]) {
			NotEqual++;
		}
	}
	return NotEqual;
}


int strncmp(const char *FirstString , const char *SecondString) {
	unsigned int i;
	int NotEqual = 0;
	for(i = 0; (FirstString[i] != '\0')||(SecondString[i] != '\0'); i++) {
		if(FirstString[i] != SecondString[i]) {
			NotEqual++;
		}
	}
	return NotEqual;
}

int atoi(const char *Buffer) {
	int Value = 0;
	int Positive = 1;
	if(*Buffer == '-') {
		Positive = -1;
		Buffer++;
	}
	if(*Buffer == '+') {
		Positive = 1;
		Buffer++;
	}
	if(*Buffer == '\0') {
		return 0;
	}
	while(*Buffer != NULL) {
		if((*Buffer >= '0') && (*Buffer <= '9')) {
			Value = Value*10+(*Buffer)-'0';
			Buffer++;
		}
	}
	if(Positive == -1) {
		Value = -Value;
	}
	return Value;
}

int atol(const char *Buffer) {
	int Value = 0;
	int Positive = 1;
	if(*Buffer == '-') {
		Positive = -1;
		Buffer++;
	}
	if(*Buffer == '+') {
		Positive = 1;
		Buffer++;
	}
	if(*Buffer == '\0') {
		return 0;
	}
	while(*Buffer != NULL) {
        Value *= 16;
		if(((*Buffer >= 'A') && (*Buffer <= 'Z'))) {
			Value += ((*Buffer)-'A')+10;
			Buffer++;
		}
        else if(((*Buffer >= 'a') && (*Buffer <= 'z'))) {
			Value += ((*Buffer)-'a')+10;
			Buffer++;
        }
        else {
            Value += ((*Buffer)-'0');
            Buffer++;
        }
	}
	if(Positive == -1) {
		Value = -Value;
	}
	return Value;
}

char *itoa(int Value , char *Buffer , int Radix) {
	int TempBuffer;
	if(Value == 0) {
		Buffer[0] = '0';
		Buffer[1] = '\0';
		return Buffer;
	}
	while(Value) {
		if(Radix <= 10) {
			*Buffer++ = (Value%Radix)+'0';
		}
		else {
			TempBuffer = Value%Radix;
			if(TempBuffer <= 9) {
				*Buffer++ = TempBuffer+'0';
			}
			else {
				*Buffer++ = TempBuffer-10+'A';
			}
		}
		Value /= Radix;
	}
	*Buffer = '\0';
	ReverseString(Buffer);
	return Buffer;
}

void ReverseString(char *Buffer) {
	int i;
	char TempBuffer;
	for(i = 0; i < strlen(Buffer)/2; i++) {
		TempBuffer = Buffer[i];
		Buffer[i] = Buffer[(strlen(Buffer)-1)-i];
		Buffer[(strlen(Buffer)-1)-i] = TempBuffer;
	}
	return;
}

void vsprintf(char *Destination , const char *Format , va_list ap) {
	int i;
	int j = 0;
	for(i = 0; i < strlen(Format); i++) {
		switch(Format[i]) {
			case '%':
				switch(Format[i+1]) {
					case 'c': {
						char Buffer = va_arg(ap , int);
						Destination[j++] = Buffer;
						i++;
						break;
					}
					case 's': {
						char *Buffer = va_arg(ap , char*);
						strcpy(Destination+j , Buffer);
						j += strlen(Buffer);
						i++;
						break;
					}
					case 'd':
					case 'i': {
						int Value = va_arg(ap , int);
						char *Buffer = {0 , };
						itoa(Value , Buffer , 10);
						ReverseString(Buffer);
						strcpy(Destination+j , Buffer);
						j += strlen(Buffer);
						i++;
						break;
					}
					case 'X': {
						unsigned long Value = va_arg(ap , unsigned long);
						char Buffer[128] = {0 , };
						itoa(Value , Buffer , 16);
						ReverseString(Buffer);
						strcpy(Destination+j , Buffer);
						j += strlen(Buffer);
						i++;
						break;
					}
					case 'f': {
						break; 
					}
					case '%': {
						Destination[j++] = '%';
						i++;
						break;
					}
					default: {
						i++;
						break;
					}
				}
				break;
			default:
				Destination[j++] = Format[i];
				break;
		}
	}
	Destination[j] = '\0';
}

void sprintf(char *Destination , const char *Format , ...) {
	va_list ap;
	va_start(ap , Format);
	vsprintf(Destination , Format , ap);
	va_end(ap);
}