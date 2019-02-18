#include "syscall.h"
#include "copyright.h"

#define MAX_LENGTH 255

int main() {
	int a = 0;
	Print("Nhap Vao 1 so nguyen: ");
	a = ReadInt();
	Print("Ket qua la: ");
	PrintInt(a);
	return 0;
	
	
}
