// # NOC Workset definitions are defined here 
#ifndef SPECDEFS_H
#define SPECDEFS_H

#define BOOL int
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DUMP "-analysis"
#define SIM "-simulate"

// As the stack is not generic FLOAT and DOUBLE are not possible at this point
typedef enum {
    // DATA TYPES
    NOC_INT,
    NOC_FLOAT,
    NOC_BYTE,
    NOC_CHAR,
    NOC_DOUBLE,
    NOC_STR, // # NOC-like-strings end with '$' but C-like-strings end with '\0'
    C_STR,

    // FUNCTIONS
    PRINT,

    // OPERATORS
    PLUS,
    MIN,
    MUL,
    DIV,
    EQ,

    // INVALIDS
    UNDEFINED
}Types;

// # Types workset
char *types[] = {"INT","FLOAT","BYTE","CHAR","DOUBLE","STR","C-STR","print","+","-","*","/","=","UNDEFINED"};
// # Types readable
char *types2[] = {"TYPE-INT","TYPE-FLOAT","TYPE-BYTE","TYPE-CHAR","TYPE-DOUBLE","TYPE-STR","TYPE-C-STR","FUNCTION","OP-PLUS","OP-MIN","OP-MUL","OP-DIV","OP-EQ","UNDEFINED"};


typedef struct lexer{
    char *raw;
    Types type;
    int val;
    int line;
}Lexeme;

// # A Fancy extension check :)
BOOL extchk(const char *file){
    BOOL isnoc = FALSE;
    int bp1 = 0;
    for(int i = 0; i < strlen(file); i++){
        if(file[i] == '.'){
            bp1 = i;
            break;
        }
    }

    char *minipath = (char*)malloc(sizeof(char)*strlen(file)+1);
    int mpath = 0;
    for(int j = bp1+1; j < strlen(file); j++){
        minipath[mpath] = file[j];
        mpath++;
    }
    minipath[mpath] = '\0';
    if(strcmp(minipath,"noc") == 0){
        isnoc = TRUE;
    }
    return isnoc;
}


BOOL is_int(char *data){
    BOOL check = FALSE;
    int expect_int = strlen(data);
    int has_int = 0;
    if(data[0] == '-'){
        if((int)strlen(data) > 1){
            has_int += 2; // # (+1) for count and another (+1) for some padding for some unknown reason
        }
    }
    for(int i = 0;i < (int)strlen(data); i++){
        if(data[i] >= '0' && data[i] <= '9'){
            has_int += 1; 
        }
        else{
            break;
        }
    }

    if(has_int == expect_int){
        check = TRUE;
    }
    return check;
}

Types typecheck(char *data){
    Types thistype;
    if(is_int(data) == TRUE){
        thistype = NOC_INT;
    }
    else if(strcmp(data,types[PLUS]) == 0){
        thistype = PLUS;
    }
    else if(strcmp(data,types[MIN]) == 0){
        thistype = MIN;
    }
    else if(strcmp(data,types[MUL]) == 0){
        thistype = MUL;
    }
    else if(strcmp(data,types[DIV]) == 0){
        thistype = DIV;
    }
    else if(strcmp(data,types[EQ]) == 0){
        thistype = EQ;
    }
    // FUNCTIONS
    else if(strcmp(data,types[PRINT]) == 0){
        thistype = PRINT;
    }
    else{
        thistype = UNDEFINED;
    }
    return thistype;
}

int get_size_of_file(const char* path)
{
    FILE *file = fopen(path,"r");
    char ch;
    int size = 0;
    while((ch = fgetc(file)) != EOF){  
        //Counts each word  
        if(ch ==' ' || ch == '\n')  
            size++;  
    }  

  return size;
}

// Parsing "raw" Data from file
Lexeme *parse_program_from_file(const char* filename){
    FILE *fp = fopen(filename,"r");
    int size = get_size_of_file(filename);
    Lexeme *lexarray = (Lexeme*)malloc(sizeof(Lexeme)*size);
    int LexN = 0;

    char delimits[] = {' ','\n','\0'};
    
    char main_buffer_frame[256];
    int readlines = 0; 
    while(fgets(main_buffer_frame,256,fp)){
        readlines += 1;
        char *mini_buffer = strtok(main_buffer_frame,delimits);
        while(mini_buffer != NULL){
            // printf("{BUFFER} %s\n",mini_buffer);
            char *min_buf = (char*)malloc(sizeof(char)*256);
            int i;
            for(i = 0;i < strlen(mini_buffer);i++){
                min_buf[i] = mini_buffer[i];
            }
            min_buf[i] = '\0'; // Closing the open string to reduce glitches

            lexarray[LexN].raw = min_buf;
            lexarray[LexN].type = typecheck(min_buf);
            if(lexarray[LexN].type == NOC_INT){
                lexarray[LexN].val = atoi(min_buf); 
            }
            lexarray[LexN].line = readlines;
            LexN++;
            mini_buffer = strtok(NULL,delimits);
        }
    }
    return lexarray;
}


#endif // SPECDEFS_H