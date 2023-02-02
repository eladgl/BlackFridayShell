#ifndef BLACK_H_
#define BLACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define BIN_PATH "/bin/"
#define BLACK_PATH "/home/braude/"
#define BUFF 256 //used in the input length - command and flags length

enum Command {
    CREATE_FLYER,   //0
    GET_FLYER,      //1
    MAKE_ORDER,     //2
    GET_NUM_COMP,   //3
    GET_ORDER_NUM,  //4
    GET_SUMMARY,    //5
    END_RUN,        //6
    COMMAND_END     //7
    };
 
static const char *command_str[COMMAND_END] = {
    "CreateFlyer",  //0
    "GetFlyer",     //1
    "MakeOrder",    //2
    "GetNumComp",   //3
    "GetOrderNum",  //4
    "GetSummary",   //5
    "exit"          //6
    };
    
int execute_bin(char * cmd, char * path);

char * strIncrease(char *str);

#endif
