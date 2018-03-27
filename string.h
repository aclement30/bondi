#ifndef STRING_H
#define STRING_H

// String replacement - move string from flash to local buffer
char * getString(const char * str);
char * getStringCopy(const char * str);

#endif