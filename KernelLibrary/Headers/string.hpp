#ifndef _STRING_H_
#define _STRING_H_

#include <Types.hpp>
#include <cstdarg>

unsigned int memset(void *Destination , int Data , unsigned int Size);
unsigned int memcpy(void *Destination , const void *Source , unsigned int Size);
int strlen(const char *Destination);
char *strcpy(char *Destination , const char *Source);
char *strcat(char *Destination , const char *Source);
int strcmp(const char *Destination , const char *Source);
int strncmp(const char *Destination , const char *Source);

unsigned long atoi(const char *Buffer);
unsigned long atol(const char *Buffer);
char *itoa(unsigned long Value , char *String , int Radix);
void ReverseString(char *Buffer);

void sprintf(char *Destination , const char *Format , ...);
void vsprintf(char *Destination , const char *Format , va_list ap);

#endif