// exception.cc
//    Entry point into the Nachos kernel from user programs.
//    There are two kinds of things that can cause control to
//    transfer back to here from user code:
//
//    syscall -- The user code explicitly requests to call a procedure
//    in the Nachos kernel.  Right now, the only function we support is
//    "Halt".
//
//    exceptions -- The user code does something that the CPU can't handle.
//    For instance, accessing memory that doesn't exist, arithmetic errors,
//    etc.
//
//    Interrupts (which can also cause control to transfer from user
//    code into the Nachos kernel) are handled elsewhere.
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
#define MAXFILELENGTH 255

//----------------------------------------------------------------------
// ExceptionHandler
//     Entry point into the Nachos kernel.  Called when a user program
//    is executing, and either does a syscall, or generates an addressing
//    or arithmetic exception.
//
//     For system calls, the following is the calling convention:
//
//     system call code -- r2
//        arg1 -- r4
//        arg2 -- r5
//        arg3 -- r6
//        arg4 -- r7
//
//    The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//    "which" is the kind of exception.  The list of possible exceptions
//    are in machine.h.
//----------------------------------------------------------------------

// IncreasePC help OS increase program counter,
// previousValue = currentValue
// currentValue = nextValue
// nextValue += 4
void IncreasePC() {
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

int System2User(int virtAddr, int length, char* buf) {
    if (length < 0)
        return -1;
    if (length == 0)
        return length;
    
    int i = 0;
    int oneChar = 0;
    
    do {
        oneChar = (int)buf[i];
        machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < length && oneChar != 0);
    
    return i;
}


//Handle All Exeption Type
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
                    
                    //default syscall of nachOS
                case SC_Halt:
                    printf("Shut Down \n");
                    interrupt->Halt();
                    return;
                  
                    //void Print(char buf[]);
                    //print string from console
                case SC_PrintString: {
                    int virtAddr;
                    char* buffer;
                    virtAddr = machine->ReadRegister(4);
                    buffer = User2System(virtAddr, 255);
                    int length = 0;
                    while (buffer[length] != 0) length++;
                    gSynchConsole->Write(buffer, length + 1);
                    delete buffer;
                    IncreasePC();
                    return;
                }
                    
                    //void Scan(char* buffer, int length);
                    //scan from console
                case SC_ReadString: {
                    int virtAddr, length;
                    char* buffer;
                    virtAddr = machine->ReadRegister(4);
                    length = machine->ReadRegister(5);
                    buffer = User2System(virtAddr, length);
                    gSynchConsole->Read(buffer, length);
                    System2User(virtAddr, length, buffer);
                    delete buffer;
                    IncreasePC();
                    return;
                }
                    
                case SC_ReadInt: {
                    char *buff = new char [MAXFILELENGTH + 1];
                    int numbytes = gSynchConsole->Read(buff, MAXFILELENGTH);
                    int number = 0;
                    
                    //check isNegative
                    bool isNegative = false;
                    int firstChar = 0;
                    
                    if(buff[0] == '-') {
                        isNegative = true;
                        firstChar = 1;
                    }
                    
                    //wrong input
                    for (int i = firstChar; i < numbytes; i++) {
                        if (buff[i] == '.' || (buff[i] < '0' && buff[i] > '9')) {
                            printf("\n The integer number is not valid");
                            machine->WriteRegister(2, 0);
                            IncreasePC();
                            delete buff;
                            return;
                        }
                    }
                    
                    //input value is an integer
                    for (int i = firstChar; i < numbytes; i++) {
                        number = number * 10 + (int)(buff[i] - 48);
                    }
                    
                    if (isNegative) {
                        number *= -1;
                    }
                    
                    machine->WriteRegister(2, number);
                    IncreasePC();
                    delete buff;
                    return;
                }
                    
                    //void PrintInt(int number)
                    //Print out an integer on screen
                case SC_PrintInt: {
                    int number = machine->ReadRegister(4);
                    
                    bool isNegative = false;
                    int length = 0;
                    int firstChar = 0;
                    
                    if (number < 0) {
                        isNegative = true;
                        number *= -1;
                        firstChar = 1;
                    } else if (number == 0)
                        length = 1;
                    
                    int temp = number;
                    
                    while(temp) {
                        length++;
                        temp /= 10;
                    }
                    
                    char *buff = new char[MAXFILELENGTH + 1];
                    for (int i = firstChar + length - 1; i >= firstChar; i--) {
                        buff[i] = (char)((number % 10) + 48);
                        number /= 10;
                    }
                    
                    if (isNegative) {
                        buff[0] = '-';
                        buff[length + 1] = 0;
                        gSynchConsole->Write(buff, length + 1);
                        delete buff;
                        IncreasePC();
                        return;
                    }
                    
                    buff[length] = 0;
                    gSynchConsole->Write(buff, length);
                    delete buff;
                    IncreasePC();
                    return;
                }
            }
            IncreasePC();
            break;
    }
}

