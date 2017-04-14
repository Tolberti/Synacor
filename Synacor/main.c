#include <stdio.h>

#include <stdlib.h>

#define Word unsigned short int // The VM addresses 16 bits at a time, neater to call it a word than an unsigned uint each time.

int main(int argc, char **argv)
{
    Word opcode; //The correct opcode being executed.
    unsigned int IsRunning = 1; // Using this as a boolean - as long as it's 1, the VM's on the go!
    unsigned int ProgramCounter = 0; //The current instruction we're executing.
    Word Memory[32775] = {0}  ; // Defines the main memory, 32768 values + 8 registers
    Word *Registers[8] = {  // Less cumbersome to have an array of pointers pointing to the addresses of the registers
        &Memory[32768],
        &Memory[32769],
        &Memory[32770],
        &Memory[32771],
        &Memory[32772],
        &Memory[32773],
        &Memory[32774],
        &Memory[32775]
    };
    Word *Stack = (Word *) malloc(4); // The stack. Initially allocated with enough memory for 2 addresses. Will reallocate on the fly.
    MallocCheck(Stack); // Make sure that the Stack was allocated okay, exit the program otherwise. This'll pop up a lot.
    FILE *BinFile; // The pointer for the binary input file
    BinFile = fopen("C:\\Users\\Josh\\Desktop\\Synacor\\challenge.bin", "rb"); // Open as read-only, binary
    fread(Memory, sizeof(Word), 32768, BinFile); //Reads the first 32768 Words from the Binary File, and dumps them in main memory. Hopefully.

    while(IsRunning)
    {
        opcode = Memory[ProgramCounter]; //Set the opcode to whatever's currently in the memory at PC address. 

        switch(opcode)
        {
            case 0: // Terminate program and virtual machine.
            IsRunning = 0;
            printf("Exiting.");
            break;
            
            case 6: // Jump
            ProgramCounter = Memory[ProgramCounter+1];
            break;
            
            case 7: // Jump if nonzero
                                              
            printf("Checking value at %u for nonzero jump to %u: %u\n", ProgramCounter+1, Memory[ProgramCounter+2], Memory[ProgramCounter+1]);                            
            if ((Memory[ProgramCounter+1]) != 0)
            {
                printf("Jumping to %u\n", Memory[ProgramCounter+2]);               
                ProgramCounter = Memory[ProgramCounter+2];
            }
            else
            {
                ProgramCounter += 3;
            }
            break;
            
            case 8: // Jump if zero
            
            printf("Checking value at %u for zero jump to %u: %u\n", ProgramCounter+1, Memory[ProgramCounter+2], Memory[ProgramCounter+1]);    

            if ( (Memory[ProgramCounter+1])== 0)
            {
                printf("Jumping to %u\n", Memory[ProgramCounter+2]);
                ProgramCounter = Memory[ProgramCounter+2];
            }
            
            else
            {
                ProgramCounter += 3;
            }
            break;
            
            case 19: // Reads a byte from the next address and prints it to the terminal as an ASCII character.
            printf(&Memory[ProgramCounter+1]);
            ProgramCounter += 2;
            break;
            
            case 21: //No operation - NOOP
            ProgramCounter++;                                         
            break;
            
            default: // If it's not any valid opcode, abort the VM.
            printf("ILLEGAL OPCODE %u AT %u\n", opcode, ProgramCounter);
            IsRunning = 0;
            
        }
    }
    
    return;
}
    
void MallocCheck(void *MallocAddress) // Checks if a malloc was successful
{
    if (MallocAddress == NULL)
    {
        printf("ERROR: MEMORY ALLOCATION FAILED\n");
        exit(1);
    }
    else 
    {
        printf("Memory allocated okay.\n");
        return;
    }
}   