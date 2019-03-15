#include "syscall.h"

int main() {
	int size;
	int a[100];
	int counter = 0;
	int tmp;
	int i = 0;
	int j = 0;
	PrintString("--------------------BUBBLE SORT--------------------\n");
	PrintString("Enter number of element: \n");
	size = ReadInt();

	for (counter = 0; counter < size; counter++) {
		PrintString("Enter value of element ");
		PrintInt(counter);
		PrintString(" : ");
		a[counter] = ReadInt();
	}

	//bubbleSort
	for (i = 0; i < size; i++)
		for (j = 0; j < size - i - 1; j++)
			if (a[j] > a[j + 1]) {
				tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
			
	//Print out to console

	i = 0;
	for (i = 0; i < size; i++) {
		PrintInt(a[i]);
		PrintString("\n");
	}
}

