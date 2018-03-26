#include <avr/pgmspace.h>
#include "string.h"

char inputString1[80]; // general purpose input string, 80 bytes 
char inputString2[80]; // general purpose input string, 80 bytes 
char outputString[80]; // general output string, 80 bytes 

char * getString(const char * str) {
  strcpy_P(outputString, (char*)str);
  return outputString;
}