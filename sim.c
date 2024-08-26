#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



#define Registers 16
#define Memory 4096
#define IO_num 23
#define Disk 16386
#define Monitor 65536
#define MAX_ROW_LENGTH 6
#define HEX_SIZE 6


typedef struct {
    bool irq0enable;
    bool irq1enable;
    bool irq2enable;
    bool irq0status;
    bool irq1status;
    bool irq2status;
    int irqhandler;
    int irqreturn;
    long clks;
    long leds;
    long display7seg;
    bool timerenable;
    long timercurrent;
    long timermax;
    int diskcmd;
    int disksector;
    int diskbuffer;
    bool diskstatus;
    int reserved0;
    int reserved1;
    int monitoraddr;
    int monitordata;
    int monitorcmd;

} IOStruct;
//IO Mapping
/*
{"irq0enable","00"},
{"irq1enable","01"},
{"irq2enable","02"},
{"irq0status","03"},
{"irq1status","04"},
{"irq2status","05"},
{"irqhandler","06"},
{"irqreturn","07"},
{"clks","08"},
{"leds","09"},
{"display7seg","0A"},
{"timerenable","0B"},
{"timercurrent","0C"},
{"timermax","0D"},
{"diskcmd","0E"},
{"disksector","0F"},
{"diskbuffer","10"},
{"diskstatus","11"},
{"reserved","12"},
{"reserved","13"},
{"monitoraddr","14"},
{"monitordata","15"},
{"monitorcmd","16"},

*/

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';       // Numerical characters
    }
    else if (c >= 'A' && c <= 'F') {
        return 10 + c - 'A';  // Uppercase hex characters
    }
    else if (c >= 'a' && c <= 'f') {
        return 10 + c - 'a';  // Lowercase hex characters
    }
    return 0; // Not a valid hex character (optional: handle as error)
}
/*
long read_irq2in(FILE* irq2in) {
    char line[100];
    char* endptr;

    // Read a line from the file and convert it to a long integer
    if (fgets(line, sizeof(line), irq2in) != NULL) {
        long num = strtol(line, &endptr, 10);
        printf("line is %c\n", *line);

        // Simple error checking: ensure we read some numbers and reached the end of the number
        if (endptr != line && (*endptr == '\n' || *endptr == '\0')) {
            return num;
        }
    }
    return -1;  // Return -1 on error or if no line is read
}

*/


void update_line_in_trace(int pc, char MEM[], long Registers_array[], FILE* trace)
{
    fprintf(trace, "%03X %s", pc , MEM);
    for (int i = 0; i < Registers; i++)
    {
        fprintf(trace, " %08X", Registers_array[i]);
    }
    fprintf(trace, "\n");
}


void HandleInInstruction(int hwAddress,int rd, long Registers_array[], FILE* output_files[], IOStruct* IO) {

    char regName[32]; // Buffer to hold register name
    int value; // To store the value of the register

    // Determine the register value based on the hardware address
    switch (hwAddress) {
    case 0: value = IO->irq0enable; strcpy(regName, "irq0enable"); break;
    case 1: value = IO->irq1enable; strcpy(regName, "irq1enable"); break;
    case 2: value = IO->irq2enable; strcpy(regName, "irq2enable"); break;
    case 3: value = IO->irq0status; strcpy(regName, "irq0status"); break;
    case 4: value = IO->irq1status; strcpy(regName, "irq1status"); break;
    case 5: value = IO->irq2status; strcpy(regName, "irq2status"); break;
    case 6: value = IO->irqhandler; strcpy(regName, "irqhandler"); break;
    case 7: value = IO->irqreturn; strcpy(regName, "irqreturn"); break;
    case 8: value = IO->clks; strcpy(regName, "clks"); break;
    case 9: value = IO->leds; strcpy(regName, "leds"); break;
    case 10: value = IO->display7seg; strcpy(regName, "display7seg"); break;
    case 11: value = IO->timerenable; strcpy(regName, "timerenable"); break;
    case 12: value = IO->timercurrent; strcpy(regName, "timercurrent"); break;
    case 13: value = IO->timermax; strcpy(regName, "timermax"); break;
    case 14: value = IO->diskcmd; strcpy(regName, "diskcmd"); break;
    case 15: value = IO->disksector; strcpy(regName, "disksector"); break;
    case 16: value = IO->diskbuffer; strcpy(regName, "diskbuffer"); break;
    case 17: value = IO->diskstatus; strcpy(regName, "diskstatus"); break;
    case 18: value = IO->reserved0; strcpy(regName, "reserved0"); break;
    case 19: value = IO->reserved1; strcpy(regName, "reserved1"); break;
    case 20: value = IO->monitoraddr; strcpy(regName, "monitoraddr"); break;
    case 21: value = IO->monitordata; strcpy(regName, "monitordata"); break;
    case 22: value = IO->monitorcmd; strcpy(regName, "monitorcmd"); break;
    default: fprintf(stderr, "Error: Invalid hardware address %d\n", hwAddress); return; // Address out of range
    }

    Registers_array[rd] = value;

    char hexString[9];


    // Write the formatted output to the file
    fprintf(output_files[6], "%d READ %s %08X\n", IO->clks, regName, Registers_array[rd]);


}


void PrintValue(IOStruct* IO, int value, FILE* file_path)
{

    // Write the formatted output to the file
    fprintf(file_path, "%ld %08X\n", IO->clks, value);

}


void HandleOutInstruction(int address, int value, FILE* output_files[],IOStruct* IO) {
    char regName[32]; // Allocate space for register name

    // Set the register value based on the address
    switch (address) {
    case 0: IO->irq0enable = value; strcpy(regName, "irq0enable"); break;
    case 1: IO->irq1enable = value; strcpy(regName, "irq1enable"); break;
    case 2: IO->irq2enable = value; strcpy(regName, "irq2enable"); break;
    case 3: IO->irq0status = value; strcpy(regName, "irq0status"); break;
    case 4: IO->irq1status = value; strcpy(regName, "irq1status"); break;
    case 5: IO->irq2status = value; strcpy(regName, "irq2status"); break;
    case 6: IO->irqhandler = value; strcpy(regName, "irqhandler"); break;
    case 7: IO->irqreturn = value; strcpy(regName, "irqreturn"); break;
    case 8: IO->clks = value; strcpy(regName, "clks"); break;
    case 9: IO->leds = value; strcpy(regName, "leds"); PrintValue(IO,value, output_files[8]); break;
    case 10: IO->display7seg = value; strcpy(regName, "display7seg"); PrintValue(IO,value, output_files[9]); break;
    case 11: IO->timerenable = value; strcpy(regName, "timerenable"); break;
    case 12: IO->timercurrent = value; strcpy(regName, "timercurrent"); break;
    case 13: IO->timermax = value; strcpy(regName, "timermax"); break;
    case 14: IO->diskcmd = value; strcpy(regName, "diskcmd"); break;
    case 15: IO->disksector = value; strcpy(regName, "disksector"); break;
    case 16: IO->diskbuffer = value; strcpy(regName, "diskbuffer"); break;
    case 17: IO->diskstatus = value; strcpy(regName, "diskstatus"); break;
    case 18: IO->reserved0 = value; strcpy(regName, "reserved0"); break;
    case 19: IO->reserved1 = value; strcpy(regName, "reserved1"); break;
    case 20: IO->monitoraddr = value; strcpy(regName, "monitoraddr"); break;
    case 21: IO->monitordata = value; strcpy(regName, "monitordata"); break;
    case 22: IO->monitorcmd = value; strcpy(regName, "monitorcmd"); break;
    default: return; // Address out of range, do nothing
    }



    // Write the formatted output to the file
    fprintf(output_files[6], "%d WRITE %s %08X\n", IO->clks, regName, value);


}



void handle_monitor(char monitor[Monitor][3], IOStruct* IO)
{
    if (IO->monitorcmd)
    {
        char pixel_to_write[3];
        snprintf(pixel_to_write,3, "%02X", IO->monitordata);
        strcpy(monitor[IO->monitoraddr], pixel_to_write);
        IO->monitorcmd = false;
    }
}



//    FILE* output_files[] = {memin, diskin, irq2in, memout, regout, trace, hwregtrace, cycles, leds, display7seg, diskout, monitor_file, monitor_yuv };

void write_all_files(char MEM[Memory][HEX_SIZE],long Registers_array[Registers], char DISK[Disk][HEX_SIZE], char monitor[Monitor][3], FILE* output_files[], IOStruct* IO, FILE* monitor_yuv)
{
    //write to memout file
    for (int i = 0; i < Memory; i++)
    {
        fprintf(output_files[3], "%05s\n",MEM[i]);
    }
    

    //write to regout
    for (int i = 2; i < Registers; i++)
    {
        fprintf(output_files[4], "%08lX\n", Registers_array[i]);
    }

    //write to  cycles
    fprintf(output_files[7], "%ld\n", IO->clks);


    //write to disk out
    for (int i = 0; i < Disk; i++)
    {
        fprintf(output_files[10], "%s\n", DISK[i]);
    }

    //write to monitor file
    char mon[3] = "00";
    for (int i = 0; i < Monitor; i++)
    {
        if (strlen(monitor[i]) ==0 )
        {
            fprintf(output_files[11], "%s\n", mon);
        }
        else
        {
            fprintf(output_files[11], "%s\n", monitor[i]);
        }
    }
    
    // write to monitor yuv file
    char byte[3] = "00";
    for (int i = 0; i < Monitor; i++)
    {
        unsigned char byte;
        byte = (unsigned char)strtol(monitor[i], NULL, 16);
        fwrite(&byte, sizeof(unsigned char), 1, monitor_yuv);
    }

    



}

int main(int argc, char* argv[])
{

    // opening input files including handling errors // ?????? sim.exe
    FILE* memin = fopen(argv[1], "r"); if (!memin) { fprintf(stderr, "Error opening file : memin.txt\n"); exit(1); }
    FILE* diskin = fopen(argv[2], "r"); if (!diskin) { fprintf(stderr, "Error opening file: diskin.txt\n"); exit(1); }
    FILE* irq2in = fopen(argv[3], "r"); if (!irq2in) { fprintf(stderr, "Error opening file: irq2in.txt\n"); exit(1); }

    // opening output files
    FILE* memout = fopen(argv[4], "w"); if (!memout) { fprintf(stderr, "Error opening file: memout.txt\n"); exit(1); }
    FILE* regout = fopen(argv[5], "w"); if (!regout) { fprintf(stderr, "Error opening file: regout.txt\n"); exit(1); }
    FILE* trace = fopen(argv[6], "w"); if (!trace) { fprintf(stderr, "Error opening file: trace.txt\n"); exit(1); }
    FILE* hwregtrace = fopen(argv[7], "w"); if (!hwregtrace) { fprintf(stderr, "Error opening file: hwregtrace.txt\n"); exit(1); }
    FILE* cycles = fopen(argv[8], "w"); if (!cycles) { fprintf(stderr, "Error opening file: cycles.txt\n"); exit(1); }
    FILE* leds = fopen(argv[9], "w"); if (!leds) { fprintf(stderr, "Error opening file: leds.txt\n"); exit(1); }
    FILE* display7seg = fopen(argv[10], "w"); if (!display7seg) { fprintf(stderr, "Error opening file: display7seg.txt\n"); exit(1); }
    FILE* diskout = fopen(argv[11], "w"); if (!diskout) { fprintf(stderr, "Error opening file: diskout.txt\n"); exit(1); }
    FILE* monitor_file = fopen(argv[12], "w"); if (!monitor_file) { fprintf(stderr, "Error opening file: monitor.txt\n"); exit(1); }
    FILE* monitor_yuv = fopen(argv[13], "wb"); if (!monitor_yuv) { fprintf(stderr, "Error opening file: monitor_yuv.txt\n"); exit(1); }

    //sim.exe memin.txt diskin.txt irq2in.txt memout.txt regout.txt trace.txt hwregtrace.txt cycles.txt leds.txt display7seg.txt diskout.txt monitor.txt monitor.yuv 
    FILE* output_files[] = { memin, diskin, irq2in, memout, regout, trace, hwregtrace, cycles, leds, display7seg, diskout, monitor_file, monitor_yuv };





    //creating all the arrays 
    long Registers_array[Registers] = { 0 }; //initialize all registers to zero
    char MEM[Memory][HEX_SIZE] = { '\0' }; //initialize MEM rows to zero
    char DISK[Disk][HEX_SIZE] = { '\0' }; // initialize disk rows to zero
    char monitor[Monitor][3] = {"00"};// initialize Monitor rows to zero



    //copying the files into the arrays
    char line[HEX_SIZE]; //5 HEXs
    int i = 0;

    while (fgets(line, sizeof(line), memin) && (i < Memory)) {
        // Remove newline character read by fgets
        line[strcspn(line, "\n")] = '\0';

        // Check if the line is not empty
            if (line[0] != '\0') {
                strcpy(MEM[i], line);
                i++;  // Increment i only if the line was copied
            }
    }

    fclose(memin);


    i = 0;
    while (fgets(line, sizeof(line), diskin) && (i < Disk)) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Check if the line is not empty
        if (line[0] != '\0') {
            strcpy(DISK[i], line);
            i++;  // Increment i only if the line was copied
        }
    }


    fclose(diskin);




    //Registers Mapping
    /*
        {"$zero",'0'}, = 0
        {"$imm",'1'}, = imm
        {"$v0",'2'},
        {"$a0",'3'},
        {"$a1",'4'},
        {"$a2",'5'},
        {"$a3",'6'},
        {"$t0",'7'},
        {"$t1",'8'},
        {"$t2",'9'},
        {"$s0",'A'},
        {"$s1",'B'},
        {"$s2",'C'},
        {"$gp",'D'},
        {"$sp",'E'},
        {"$ra",'F'},
    */

    //opcode Mapping
    /*
        {"add","00"},
        {"sub","01"},
        {"mul","02"},
        {"and","03"},
        {"or","04"},
        {"xor","05"},
        {"sll","06"},
        {"sra","07"},
        {"srl","08"},
        {"beq","09"},
        {"bne","0A"},
        {"blt","0B"},
        {"bgt","0C"},
        {"ble","0D"},
        {"bge","0E"},
        {"jal","0F"},
        {"lw","10"},
        {"sw","11"},
        {"reti","12"},
        {"in","13"},
        {"out","14"},
        {"halt","15"},
    */



    int pc = 0;
    int rd, rs, rt; //0-15
    int opcode; //0-21
    int imm = 0;
    int disk_counter = 0;
    bool halt = false; //set stop to False
    bool irq = false; //set irq to False 
    bool handle_irq = false; //flag to check if I'm already in irq
    IOStruct IO = { 0 };
    //long irq2 = read_irq2in(irq2in);
    long irq2 = -1;


    // Read integers from the file
    if (fgets(line, sizeof(line), irq2in) != NULL) {
        // Try to parse an integer from the line
        if (sscanf(line, "%ld", &irq2) == 1) {
           // printf("managed to save %ld\n", irq2); ??
        }
    }
    else {
        // Check for read errors or end of file
        if (feof(irq2in)) {
            printf("End of file reached.\n");
        }
    }




    while ((!halt) && (pc < 4096))
    {

        irq = (IO.irq0enable && IO.irq0status) || (IO.irq1enable && IO.irq1status) || (IO.irq2enable && IO.irq2status);
        if (irq && !handle_irq)//  jumping to interupt
        {
            IO.irqreturn = (pc); //save irq return pc+1
            pc = IO.irqhandler; // jump to the interupt

            handle_irq = true; //get into handle irq mode (until reti opcode is used)
        }

        else
        {

            //checking 3 interupts
            if ((IO.clks == irq2)) //irq2
            {

                if (IO.irq2enable)
                {
                    IO.irq2status = true;

                }

                //keep reading the file even if irq2 is disabled so we can keep track if it will be enabled
                if (fgets(line, sizeof(line), irq2in) != NULL) {
                    if (sscanf(line, "%ld", &irq2) == 1) {
                    }
                }
                else {
                    // Check for read errors or end of file
                    if (feof(irq2in)) {
                    }
                }

            }

            if ((IO.timercurrent == IO.timermax) && (IO.timerenable)) //irq0
            {
                IO.irq0status == true;
                IO.timercurrent = 0;
            }

            if (IO.diskcmd != 0)  //irq1 
            {
                if (!IO.diskstatus) //first time diskcmd!=0
                {
                    IO.diskstatus = true;
                    int disk_location = IO.disksector * 128;
                    if (IO.diskcmd == 1) //read
                    {
                        

                        for (int i = 0; i < 128; i++) //copying all the 128 rows in this specific sector
                        {

                            strncpy(MEM[IO.diskbuffer + i], DISK[disk_location + i],5); // copying from disc to mem

                        }
                    }
                    if (IO.diskcmd == 2) //write
                    {

                        for (int i = 0; i < 128; i++) //writing all the 128 rows in this specific sector
                        {
                            if ((IO.diskbuffer + i) > 1127)
                            {
                                printf("2. the disk was readed from %d with value %s\n", disk_location + i,MEM[IO.diskbuffer + i]);

                            }
                            strncpy(DISK[disk_location + i],MEM[IO.diskbuffer + i],5); // writing from mem to disc

                        }
                    }
                }
                else if (disk_counter == 1023) //irq1 is happenning, 1023 because on first time counter is not incremeting
                {
                    //initial all statuses 
                    IO.irq1status = true;
                    IO.diskstatus = false;
                    IO.diskcmd = 0;
                    disk_counter = 0;
                }
                else //disc is still qorking
                {
                    disk_counter++;
                }


            }



            //handle monitor (can also be handled in the out function but I keep it here so it will be more clear)
            handle_monitor(&monitor, &IO);






            int firstDigit = hexCharToInt(MEM[pc][0]);
            int secondDigit = hexCharToInt(MEM[pc][1]);

            // Calculate opcode using hexadecimal logic
            int opcode = (firstDigit * 16) + secondDigit;

            rd = hexCharToInt(MEM[pc][2]);
            rs = hexCharToInt(MEM[pc][3]);
            rt = hexCharToInt(MEM[pc][4]);

            //imm check
            if ((rd == 1) || (rs == 1) || (rt == 1))
            {
                pc++;
                long imm = 0;
                imm = strtol(MEM[pc], NULL, 16);
                if (imm & 0x80000) {  // Check if the 20th bit is set
                    imm -= 0x100000;  // Subtract 2^20 to adjust the range
                }
                Registers_array[1] = imm;
            }

            //write to trace
            update_line_in_trace(pc-1, MEM[pc-1], Registers_array, output_files[5]); 


            switch (opcode)
            {
            case 0: //add
                Registers_array[rd] = Registers_array[rs] + Registers_array[rt];
                break;
            case 1: //sub
                Registers_array[rd] = Registers_array[rs] - Registers_array[rt];
                break;
            case 2: //mul
                Registers_array[rd] = Registers_array[rs] * Registers_array[rt];
                break;
            case 3: //and
                Registers_array[rd] = Registers_array[rs] & Registers_array[rt];
                break;
            case 4: //or
                Registers_array[rd] = Registers_array[rs] | Registers_array[rt];
                break;
            case 5: //xor
                Registers_array[rd] = Registers_array[rs] ^ Registers_array[rt];
                break;
            case 6: //sll
                Registers_array[rd] = (long)(unsigned long)(Registers_array[rs] << Registers_array[rt]);
                break;
            case 7: //sra
                Registers_array[rd] = Registers_array[rs] >> Registers_array[rt];
                break;
            case 8: //srl
                Registers_array[rd] = (long)(unsigned long)(Registers_array[rs] >> Registers_array[rt]);
                break;
            case 9: //beq
                if (Registers_array[rs] == Registers_array[rt])
                {
                    pc = Registers_array[rd]-1;
                }
                break;
            case 10: //bne
                if (Registers_array[rs] != Registers_array[rt])
                {
                    pc = Registers_array[rd]-1;
                }
                break;
            case 11: //blt
                if (Registers_array[rs] < Registers_array[rt])
                    pc = Registers_array[rd]-1;
                break;
            case 12: //bgt
                if (Registers_array[rs] > Registers_array[rt])
                    pc = Registers_array[rd]-1;
                break;
            case 13: //ble
                if (Registers_array[rs] <= Registers_array[rt])
                    pc = Registers_array[rd]-1;
                break;
            case 14: //bge
                if (Registers_array[rs] >= Registers_array[rt])
                    pc = Registers_array[rd]-1;
                break;
            case 15: //jal
                Registers_array[rd] = pc + 1; //link the next instruction
                pc = (int)Registers_array[rs]-1; //jump to rs
                break;
            case 16: //lw
                if ((Registers_array[rs] + Registers_array[rt]) < Memory) {
                    // Convert hexadecimal to long integer
                    Registers_array[rd] = (long)strtol(MEM[(Registers_array[rs] + Registers_array[rt])], NULL, 16);
                }
                else {
                    printf("Index out of bounds\n");
                    printf("rs is %d, rt is %d, rd is %d\n", Registers_array[rs], Registers_array[rt], Registers_array[rd]);
                }
                break;
            case 17: //sw
                sprintf(MEM[Registers_array[rs] + Registers_array[rt]], "%05X", Registers_array[rd]);
                break;
            case 18: //reti
                pc = (IO.irqreturn)-1;
                handle_irq = false;
                break;
            case 19: //in
                HandleInInstruction((Registers_array[rs] + Registers_array[rt]), rd, Registers_array, output_files, &IO); //sending to function handling the "in"  (the hw address, the register rd,array of registers, IO typedef address) 
                break;
            case 20: //out 
                HandleOutInstruction((Registers_array[rs] + Registers_array[rt]), Registers_array[rd], output_files, &IO); // (hardware address, value, address of IO typedef)
                break;
            case 21: //HALT
                halt = 1;
                pc = 4096;
                break;


            }
            //increments
            pc++;
            //printf("pc is %d\n", pc);
            IO.timercurrent++;
            if (IO.clks == 0b11111111111111111111111111111111)
            {
                IO.clks = 0;
            }
            else
            {
                IO.clks++;
            }
        } 




    } //while close loop
    
    //LOAD ALL FILES
    printf("finished writing all files");
    write_all_files(MEM, Registers_array, DISK, monitor, output_files, &IO, monitor_yuv);

    for (int i = 0; i < 1200; i++)
    {
        printf("the disk mem in location %d is %s\n", i, MEM[i]);

    }

    // close all files
    for (int i = 0; i < sizeof(output_files) / sizeof(output_files[0]); i++)
    {
        if (output_files[i])
        {
            fclose(output_files[i]);
        }
    }
    fclose(monitor_yuv);
    return 0;

}


