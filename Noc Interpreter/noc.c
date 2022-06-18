// # NOC: A stack based programming language
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./includes/specdefs.h"

void analyse(const char *filepath){
    printf("Lexical Analysis %s\n",filepath);
    Lexeme *lexlist = parse_program_from_file(filepath);

    for(int x = 0;(lexlist[x].type >= 0 && lexlist[x].type <= 13); x++){
        printf("(<%s,%s>)\n",lexlist[x].raw,types2[lexlist[x].type]);
    }
} 

void simmsg(const char *filepath){
    printf("[NOC] Simulating File: %s\n",filepath);
}


void simulate(const char *filepath,char *sim_pattern)
{   
    Lexeme *lexlist = parse_program_from_file(filepath);
    simmsg(filepath);

    if(strcmp(sim_pattern,"-safe") == 0){
        printf("[NOC] Simulating SafeCode\n");
        printf("[CMD] Safeties Enabled\n\n");
    }
    else if(strcmp(sim_pattern,"-unsafe") == 0){
        printf("[NOC] Simulating UnsafeCode\n");
        printf("[CMD] Safeties Disabled\n\n");
    }

    int *stack = (int*)malloc(sizeof(int)*get_size_of_file(filepath));
    int sp = -1;

    for(int x = 0;(lexlist[x].type >= 0 && lexlist[x].type <= 13); x++){
        if(lexlist[x].type == NOC_INT){
            sp += 1;
            stack[sp] = lexlist[x].val;
        }
        else if(lexlist[x].type == PLUS){
            if(sp >= 1){
                int a = stack[sp];
                sp -= 1;
                int b = stack[sp];
                sp -= 1;
                sp += 1;
                stack[sp] = b + a;
            }
            else if(sp < 1){
                if(strcmp(sim_pattern,"-safe") == 0){
                    printf("%s:%d WARNING: Stack has less elements to operate\n",filepath,lexlist[x].line);
                }
                else if(strcmp(sim_pattern,"-unsafe") == 0){
                    int a = stack[sp];
                    sp -= 1;
                    int b = stack[sp];
                    sp -= 1;
                    sp += 1;
                    stack[sp] = b + a;
                }
            }
        }
        else if(lexlist[x].type == MIN){
            if(sp >= 1){
                int a = stack[sp];
                sp -= 1;
                int b = stack[sp];
                sp -= 1;
                sp += 1;
                stack[sp] = b - a;
            }
            else if(sp < 1){
                if(strcmp(sim_pattern,"-safe") == 0){
                    printf("%s:%d WARNING: Stack has less elements to operate\n",filepath,lexlist[x].line);
                }
                else if(strcmp(sim_pattern,"-unsafe") == 0){
                    int a = stack[sp];
                    sp -= 1;
                    int b = stack[sp];
                    sp -= 1;
                    sp += 1;
                    stack[sp] = b - 1;
                }
            }
        }
        else if(lexlist[x].type == MUL){
            if(sp >= 1){
                int a = stack[sp];
                sp -= 1;
                int b = stack[sp];
                sp -= 1;
                sp += 1;
                stack[sp] = b * a;
            }
            else if(sp < 1){
                if(strcmp(sim_pattern,"-safe") == 0){
                    printf("%s:%d WARNING: Stack has less elements to operate\n",filepath,lexlist[x].line);
                }
                else if(strcmp(sim_pattern,"-unsafe") == 0){
                    int a = stack[sp];
                    sp -= 1;
                    int b = stack[sp];
                    sp -= 1;
                    sp += 1;
                    stack[sp] = b * a;
                }
            }
        }
        else if(lexlist[x].type == DIV){
            if(sp >= 1){
                int a = stack[sp];
                sp -= 1;
                int b = stack[sp];
                sp -= 1;
                sp += 1;
                stack[sp] = b / a;
            }
            else if(sp < 1){
                if(strcmp(sim_pattern,"-safe") == 0){
                    printf("%s:%d WARNING: Stack has less elements to operate\n",filepath,lexlist[x].line);
                }
                else if(strcmp(sim_pattern,"-unsafe") == 0){
                    int a = stack[sp];
                    sp -= 1;
                    int b = stack[sp];
                    sp -= 1;
                    sp += 1;
                    stack[sp] = b / a;
                }
            }
        }

        else if(lexlist[x].type == PRINT){
            if(sp > -1){
                printf("%d\n",stack[sp]);
                sp -= 1; 
            }
            else if(sp < 0){
                if(strcmp(sim_pattern,"-safe") == 0){
                    printf("%s:%d WARNING: Can not write empty stack\n",filepath,lexlist[x].line);
                }
                else if(strcmp(sim_pattern,"-unsafe") == 0){
                    printf("%d\n",stack[sp]);
                    sp -= 1;
                }
            }        
        }
        else if(lexlist[x].type == EQ){
            if(sp >= 1){
                int a = stack[sp];
                sp -= 1;
                int b = stack[sp];
                sp -= 1;

                if(b==a){
                    sp += 1;
                    stack[sp] = TRUE;
                }
                else{
                    sp += 1;
                    stack[sp] = FALSE; 
                }
            }
            else if(sp < 1){
                if(strcmp(sim_pattern,"-safe") == 0){
                    printf("%s:%d WARNING: Stack has less elements to operate\n",filepath,lexlist[x].line);
                }
                else if(strcmp(sim_pattern,"-unsafe") == 0){
                    int a = stack[sp];
                    sp -= 1;
                    int b = stack[sp];
                    sp -= 1;

                    if(b==a){
                        sp += 1;
                        stack[sp] = TRUE;
                    }
                    else{
                        sp += 1;
                        stack[sp] = FALSE; 
                    }
                }
            }
        }
        else if(lexlist[x].type == UNDEFINED){
            printf("%s:%d: ERROR: Undefined keyword %s\n",filepath,lexlist[x].line,lexlist[x].raw);
            printf("Simulation terminated\n");
            exit(0);
        }
    }
}

int main(int argc,char **argv)
{   
    if(argc >= 4){
        const char *filepath = argv[1];
        char *mode = argv[2];
        char *codetype = argv[3];

        if((strcmp(codetype,"-safe") == 1) && (strcmp(codetype,"-unsafe") == 1)){
            printf("[NOC] ERROR: Undefined CodeType %s\n",codetype);
            printf("[NOC] Terminated\n");
            exit(0);
        }

        FILE *file = fopen(filepath,"r");
        if(!file){
            printf("ERROR: Expected a valid input file\n");
            printf("FOUND: %s\n",filepath); 
            printf("[NOC] Terminated\n");
            exit(0);
        }

        // # This is a fancy extension check :)
        if(extchk(filepath) == FALSE){
            printf("ERROR: Noc can't process this type of file\n");
            printf("FOUND: %s\n",filepath);
            printf("[NOC] Terminated\n");
            exit(0);
        }

        // # For now it only supports 1 mode at a time
        // # In Future: It will be supporting multiple modes at a time
        if(strcmp(mode,SIM) == 0){
            simulate(filepath,codetype);
        }
        else if(strcmp(mode,DUMP) == 0){
            analyse(filepath);
        }
        else{
            printf("ERROR: Unknown subcommand \"%s\"\n",mode);
            printf("Usage: noc <FILE> [SUBCOMMAND]\n");
            printf("SUBCOMMAND:\n");
            printf("   -simulate\tSimulate the program\n");
            printf("   -analysis\tAnalyse the parsed lexemes\n");
            printf("CODETYPE:\n");
            printf("   -safe\tSimulate the code in safe environment\n");
            printf("   -unsafe\tSimulate the code in unsafe environment\n");
            exit(0);
        }
    }
    else if(argc < 4){
        printf("Usage: noc <FILE> [SUBCOMMAND] [CODETYPE]\n");
        printf("SUBCOMMAND:\n");
        printf("   -simulate\tSimulate the program\n");
        printf("   -analysis\tAnalyse the parsed lexemes\n");
        printf("CODETYPE:\n");
        printf("   -safe\tSimulate the code in safe environment\n");
        printf("   -unsafe\tSimulate the code in unsafe environment\n");
        exit(0);
    }
    return 0;
}