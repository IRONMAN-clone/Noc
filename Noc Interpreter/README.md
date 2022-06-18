# NOC
========================================================================================
Noc is a stack based programming language just like Forth or my last language Pitt     | (which i accidentally broke completely so decided to work with a new better version of |it)                                                                                    |
========================================================================================


# Simulation Guide
Usage: noc <FILE> [SUBCOMMAND] [CODETYPE]
SUBCOMMAND:
   -simulate    Simulate the program
   -analysis    Analyse the parsed lexemes
CODETYPE:
   -safe        Simulate the code in safe environment
   -unsafe      Simulate the code in unsafe environment

# Updates On The Language

# DATE :- 6/18/2022 
Added basic stack operation and integer literals
```c
34 35 + print
-1 1 + print
-5 -2 + print 
-1 -1 - print 
4 2 / print
4 2 - print 
4 2 * print 
```

# NOTICE 
If you don't want to mess up with code in this language make sure you use "-safe" simulation mode
It has all the safeties to save you from ruining the code