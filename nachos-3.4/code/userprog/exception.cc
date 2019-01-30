// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//Define area
#define MAXFILELENGHTH 255

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void InscreasePC() {
	machine->registers[PrevPCReg] = machine->registers[PCReg];
	machine->registers[PCReg] = machine->registers[NextPCReg];
	machine->registers[NextPCReg] += 4;
}

char* User2System(int virAddr, int limit) {
	int index;
	int oneChar;
	char* kernelBuf = new char[limit + 1];
	
	if (!kernelBuf) 	
		return kernelBuf;
	
	memset(kernelBuf, 0, limit + 1);

	for (index = 0; index < limit; index++) {
	machine -> ReadMem(virAddr + index, 1, &oneChar);
	kernelBuf[index] = (char)oneChar;

	if (oneChar == 0)
		 break;
	}
	return kernelBuf;
}



void ExceptionHandler(ExceptionType which) {
    int type = machine->ReadRegister(2);

	switch (which) {

	case NoException: 
		return;

	case PageFaultException: 
		printf("No valid translation found");
		interrupt->Halt();
		break;

	case ReadOnlyException: 
		printf("Write attempted to page marked read-only");
		interrupt->Halt();
		break;

	case BusErrorException: 
		printf("Translation resulted in an invalid physical address");
		interrupt->Halt();
		break;

	case AddressErrorException: 
		printf("Unaligned reference or one that was beyond the end of the address space");
		interrupt->Halt();
		break;

	case OverflowException: 
		printf("Integer overflow in add or sub.");
		interrupt->Halt();
		break;

	case IllegalInstrException:
		printf("Unimplemented or reserved instr.");
		interrupt->Halt();
		break;

	case NumExceptionTypes:
		printf("NumException types");
		interrupt->Halt();
		break;

	case SyscallException: 
		switch (type) {
		case SC_Halt:
			printf("Shut Down");
			interrupt->Halt();
			return;

		case SC_Create:	
			int virAddr;
			char* filename;

			virAddr = machine -> ReadRegister(4);
			filename = User2System(virAddr, MAXFILELENGHTH + 1);

			if (strlen(filename) == 0) {
				printf("File name is not valid");
				machine -> WriteRegister(2, -1);
				delete[] filename;
				break;
			}

			if (!filename) {
				printf("Not enough memory in system");
				machine -> WriteRegister(2, -1);
				delete[] filename;
				break;	
			}

			if (!fileSystem->Create(filename, 0)) {
				printf("Error create file");
				machine -> WriteRegister(2, -1);
				delete[] filename;
				break;		
			}
			
			printf("Create file Success");
			machine -> WriteRegister(2, 0);
			delete[] filename;
			break;
		}

		InscreasePC();
		break;
    }
}