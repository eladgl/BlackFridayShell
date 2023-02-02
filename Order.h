#ifndef ORDER_H_
#define ORDER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

//create defines
#define CSV "_csv"
#define TXT ".txt"
#define STOP "STOP"
#define NIS "NIS"
#define PATH "/home/braude/Black_Friday/"
#define GETFLYER "/home/braude/GetFlyer"
#define ORDER "_Order"
#define FLYER "GetFlyer" //this is the name if the executable
#define BUFF 128

#define ERROR(...) Error(__VA_ARGS__)

void Error(char* msg, char* mem1, char* mem2, char* mem3, char* mem4, char* mem5, char* mem6);

char* get_csv_item(char* line);

float get_csv_price(char* line, float discount);

float getDiscount(char *line);

char *ltrim(char *str);

char *rtrim(char *str);

char *trim(char *str);

char* read_file(char* path);
/*converts string to num used in discount and in quantity atoi is not used because text is not const*/ 
float convertStrToNum(char* text);

char* getQty(char *order, int delimeter);

char* getItem(char* order, int delimeter);

int write_to_file(int fd, char* str, int length);

#endif