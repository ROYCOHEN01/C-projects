#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABELS 4096
#define LABEL_NAME_LENGTH 50
#define MAX_LENGTH 10
#define MAX_ROW_LENGTH 500
#define NUM_REGISTERS 16
#define NUM_OPCODE 22

typedef struct {
    char name[LABEL_NAME_LENGTH];
    unsigned int address;
} Label;

typedef struct {
    Label labels[MAX_LABELS];
    int count;
} LabelTable;

typedef struct {
    char opcode[MAX_LENGTH];
    char rd[MAX_LENGTH];
    char rs[MAX_LENGTH];
    char rt[MAX_LENGTH];
    int immediate;
    char label_name[LABEL_NAME_LENGTH];
    int format; // 0 for R, 1 for I
} AssemblyLine;


typedef struct {
    char reg_name[MAX_LENGTH];
    char hex;
} Registers_mapping;

typedef struct {
    char opcode_name[MAX_LENGTH];
    char hex[3];
} OPCODE_mapping;




void print_assemply_line(AssemblyLine* line)
{
    printf("opcode: %s\nrd: %s\nrs: %s\nrt: %s\nimmediate: %d\nlabel name %s\nformat: $d \n", line->opcode, line->rd, line->rs, line->rt, line->immediate, line->label_name, line->format);
}

int line_type(char* start) //1 if regular line. 0 if comment or empty line. 2 if the line is .word
{
    while ((*start != '\0') && (*start != '#'))
    {
        if (*start == '.')
            return 2; //.word

        if (*start == '$')
            return 1; //regular line

        start++;
    }
    return 0; //for sure empty line 
}

void strip_whitespace(char* str)
{
    char* read = str;
    char* write = str;

    while (*read != '\0') {
        if (!isspace((unsigned char)*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}//removes white spaces and tabs

char register_name_to_string(char* register_name, Registers_mapping* register_table) //gets the register name and the register table and replace the name with the hex
{
    strip_whitespace(register_name);
    for (int i = 0; i < NUM_REGISTERS; i++)
    {
        if (strncmp(register_name, register_table[i].reg_name, strlen(register_name)) == 0)
        {
            return register_table[i].hex;
        }
    }
    int i = 0;
    printf("Register name %s not found.\n", register_name);
    return '0';

}

void opcode_name_to_hex(char* opcode_name, OPCODE_mapping* opcode_table, char* hex0, char* hex1) // gets the opcode name and the opcode table and replace the name with the hex
{
    strip_whitespace(opcode_name); //removes white spaces and tabs
    for (int i = 0; i < NUM_OPCODE; i++)
    {
        if (strncmp(opcode_name, opcode_table[i].opcode_name, strlen(opcode_name)) == 0)
        {
            *hex0 = opcode_table[i].hex[0]; //ERRORRRR
            *hex1 = opcode_table[i].hex[1];
            return;
        }
    }
    printf("opcode name %s not found\n", opcode_name);
    return;

}

int label_name_to_string(char* label_name, LabelTable* label_table) // gets the label name and the label table and replace the name with the address
{

    strip_whitespace(label_name); //removes white spaces and tabs
    for (int i = 0; i < label_table->count; i++)
    {
        if (strncmp(label_name, label_table->labels[i].name, strlen(label_name)) == 0)
        {
            return label_table->labels[i].address;
        }
    }

    printf("Label name %s not found\n", label_name);
    return 0;

}

void parse_assembly_line(char* start, AssemblyLine* line)
{

    // Initialize registers to empty strings and format to 0

    memset(line->opcode, 0, sizeof(line->opcode));
    memset(line->rd, 0, sizeof(line->rd));
    memset(line->rs, 0, sizeof(line->rs));
    memset(line->rt, 0, sizeof(line->rt));
    line->immediate = 0;
    memset(line->label_name, 0, sizeof(line->label_name));
    line->format = 0;


    int length = 0;

    // opcode
    while ((*start == ' ') || (*start == '\t'))
    {
        start++;
    }
    char* end = start;
    while (*end != '$')
    {
        end++;
    }


    length = end - start;
    memcpy(line->opcode, start, length); //copy opcode

    start = end;




    // $rd
    while ((*start == '$'))
    {
        start++;
    }
    start = end;
    while (*end != ',')
    {
        end++;
    }


    length = end - start;
    memcpy(line->rd, start, length); //copy first register



    end++;
    start = end;


    // $rs
    while ((*start == '$'))
    {
        start++;
    }
    start = end;
    while (*end != ',')
    {
        end++;
    }



    length = end - start;
    memcpy(line->rs, start, length); //copy second register

    end++;
    start = end;


    // $rt
    while ((*start == '$'))
    {
        start++;
    }
    start = end;
    while (*end != ',')
    {
        end++;
    }


    length = end - start;
    memcpy(line->rt, start, length); //copy third register

    end++;
    start = end;


    // immediate / label
    while ((*start == ' '))
    {
        start++;
    }
    if (((*start <= '9') && (*start >= '0')) || (*start == '-'))
    {
        if (*(start + 1) != 'x') //so num is represented in Decimal
        {
            line->immediate = atoi(start); //copy imm from the first number (only if immediate is a number)

        }
        else //so it means num is represented as Hex
        {
            line->immediate = strtol(start + 2, NULL, 16); // convert HEX to DEC
        }
    }

    else
    {
        end = start;
        while ((*end != ' ') && (*end != '\0') && (*end != '#'))
        {
            end++;
        }
        length = end - start;

        //printf("the length is %d", length);
        //NOTICE length calculation is not perfect...
        memcpy(line->label_name, start, length); //copy the label name. length-3 is a bit fishy so I removed it


    }


    // determinate the type
    if (strstr(line->rd, "$imm") != NULL || strstr(line->rs, "$imm") != NULL || strstr(line->rt, "$imm") != NULL)
    {
        line->format = 1; //I format
    }
    else
    {
        line->format = 0; //R format
    }


}

void pass_one(const char* filename, LabelTable* labelTable)
{
    //opening the file
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
    }

    char line[MAX_ROW_LENGTH];
    unsigned int address = 0; //the line number after the conversion
    int origin_row_count = -1; //the row number on the original file


    while (fgets(line, sizeof(line), file)) {

        char* start = line, * end = line; //initialize the pointers to slice the strings
        origin_row_count++;
        line[strcspn(line, "\n")] = 0;   // Remove newline character from line

        // Check if the line contains a label and if so, extract it
        char* label = start;

        while ((*label != '\0') && (*label != ':') && (*label != '#')) //checking if there is a label in this row
        {
            label++;
        }

        if (*label == ':') //if the while -loop stopped due to ':'
        {

            while (*start == ' ')
            {
                start++;
            }
            end = start;
            while (*end != ':')
            {
                end++;
            }
            strncpy(labelTable->labels[labelTable->count].name, start, end - start); //copy label's name into lableTable
            labelTable->labels[labelTable->count].address = address; // copy label's address into lableTable
            labelTable->count++; // Increment label count

            //moving to the start of the next word (if exists) 
            end++;
            start = end;

        }
        int type = line_type(start);
        if (type == 1) //regular line
        {
            AssemblyLine lineStruct;
            parse_assembly_line(start, &lineStruct);
            if (lineStruct.format == 1) {
                address++; // Increment address by 1 for I format
            }
            address++; // Increment address by 1 for all formats



            print_assemply_line(&lineStruct);
            printf("%d\n", address);




        }

        else if (type == 2) // .word line
        {
            //impliment the .word funtion
            address += 2; // .word is converted to 2 lines in memin
        }



    }

    fclose(file);
}

void pass_two(const char* filename, LabelTable* labelTable, Registers_mapping* register_table, OPCODE_mapping* opcode_table, const char* memin)
{

    //opening the origin file to read
    FILE* origin_file = fopen(filename, "r");
    if (!origin_file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
    }


    char line[MAX_ROW_LENGTH];
    char* pointer;
    char* label;
    char dest_file_arr[MAX_LABELS][6]; //4096 rows of 5-HEX
    int row_counter = 0;


    for (int i = 0; i < MAX_LABELS; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            dest_file_arr[i][j] = '0';
        }
        dest_file_arr[i][5] = '\0';
    }


    while (fgets(line, sizeof(line), origin_file))
    {
        pointer = line;
        label = line;

        // Check if the line contains a label and if so, extract it
        while ((*label != '\0') && (*label != ':') && (*label != '#')) //checking if there is a label in this row
        {
            label++;
        }

        if (*label == ':') //if the while -loop stopped due to ':'
        {
            pointer = label;
        }


        int type = line_type(pointer);
        if (type == 1) //regular line
        {
            AssemblyLine lineStruct;
            parse_assembly_line(pointer, &lineStruct);

            char register_hex;

            char hex0, hex1;

            opcode_name_to_hex(lineStruct.opcode, opcode_table, &hex0, &hex1);

            dest_file_arr[row_counter][0] = hex0;
            dest_file_arr[row_counter][1] = hex1;

            register_hex = register_name_to_string(lineStruct.rd, register_table); //convert rd to HEX
            dest_file_arr[row_counter][2] = register_hex;


            register_hex = register_name_to_string(lineStruct.rs, register_table); //convert rs to HEX
            dest_file_arr[row_counter][3] = register_hex;


            register_hex = register_name_to_string(lineStruct.rt, register_table); //convert rt to HEX
            dest_file_arr[row_counter][4] = register_hex;



            if (lineStruct.format == 1) //need to insert the immediate
            {
                row_counter++;
                char hex_str[6];
                if (lineStruct.label_name[0] == '\0') //means no label was saved
                {
                    sprintf_s(hex_str, sizeof(hex_str), "%05X", lineStruct.immediate & 0xFFFFF);

                }
                else
                {
                    int address = label_name_to_string(lineStruct.label_name, labelTable); //return the address of the label
                    sprintf_s(hex_str, sizeof(hex_str), "%05X", address & 0xFFFFF);

                }
                for (int i = 0; i < 5; i++)
                {
                    dest_file_arr[row_counter][i] = hex_str[i]; // inserts the imm to a new line in memin
                }


            }

            row_counter++; //increment if instruction row


        }
        else if (type == 2) // .word line
        {

            //jump over the .word
            while (*pointer != '.')
            {
                pointer++;
            }
            pointer += 5;

            //extract the row number
            int new_row_counter = 0;
            printf("row counter= %d\n", new_row_counter);
            while ((*pointer > '9') || (*pointer < '0'))
            {
                pointer++;
                printf("row counter= %d\n", new_row_counter);


            }
            if (*(pointer + 1) != 'x') //so address is represented in Decimal
            {
                while ((*pointer <= '9') && (*pointer >= '0'))
                {
                    new_row_counter = new_row_counter * 10 + ((*pointer) - '0');
                    printf("row counter= %d\n", new_row_counter);
                    pointer++;
                }
            }
            else //so it means address is represented as Hex
            {
                new_row_counter = strtol(pointer + 2, NULL, 16); // convert HEX to DEC


            }
            while ((*pointer != ','))
            {
                pointer++;

            }

            printf("!!!!!!!!!!!!!!!!!!!!!! %c\n", *pointer);

            //extract the word
            while ((*pointer > '9') || (*pointer < '0'))
            {
                pointer++;

            }
            int value = 0;
            if (((*pointer <= '9') && (*pointer >= '0')) || (*pointer == '-'))
            {


                if (*(pointer + 1) != 'x') //so num is represented in Decimal
                {
                    value = atoi(pointer); //copy imm from the first number (only if immediate is a number)
                }
                else //so it means num is represented as Hex
                {
                    value = strtol(pointer + 2, NULL, 16); // convert HEX to DEC
                }
            }
            char hex_str[6];
            sprintf_s(hex_str, sizeof(hex_str), "%05X", value & 0xFFFFF);

            printf("the number of the row is %d, the value is %s\n", new_row_counter, hex_str);
            strcpy(dest_file_arr[new_row_counter], hex_str);




            row_counter++; //increment if instruction row


        }




    }

    //opening the destination file to write

    FILE* dest_file = fopen(memin, "w");
    if (!dest_file) {
        printf("Error opening file!\n");
        return 1;
    }
    //insert the array into the file
    printf("SUCCESS1\n");
    char hex_word[6];
    for (int row = 0; row < MAX_LABELS; row++)
    {
        for (int col = 0; ((col < 5) && (dest_file_arr[row][col] != '\0')); col++)
        {
            hex_word[col] = dest_file_arr[row][col];
            printf("%c", dest_file_arr[row][col]);
        }
        hex_word[5] = '\0';
        fprintf(dest_file, "%s\n", hex_word);
        printf("\n");
    }


    fclose(origin_file);
    fclose(dest_file);
    printf("SUCCESS");

}



int main(int argc, char* argv[]) {
    LabelTable labelTable = { .count = 0 };

    const char* input_filename = argv[1];
    const char* output_filename = argv[2];

    pass_one(input_filename, &labelTable);

    for (int i = 0; i < labelTable.count; i++) {
        printf("Label %d: %s and the address is %d \n", i, labelTable.labels[i].name, labelTable.labels[i].address);
    }

    Registers_mapping register_table[NUM_REGISTERS] = //creating the registers mapping table
    {
        {"$zero",'0'},
        {"$imm",'1'},
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
    };

    OPCODE_mapping opcode_table[NUM_OPCODE] = // creating the OPCODE mapping 
    {
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
    };




    pass_two(input_filename, &labelTable, &register_table, &opcode_table, output_filename);

    return 0;
}
