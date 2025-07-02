#include <stdio.h>
#include <string.h>
#include "typeStringT.h"

// Creates a tStringT from a C string
tStringT CreateString(const char *str) {
    tStringT newString;

    // Set length to -1 to indicate creation failure by default
    newString.length = -1;

    // If input string is too long, return invalid string
    if (strlen(str) > 100) {
        return newString;
    }

    // Copy string and set length
    strcpy(newString.string, str);
    newString.length = strlen(str);

    // Mark end with '.' to indicate it's not a C string
    newString.string[newString.length] = '.';

    return newString;
}

// Concatenates two tStringT strings
tStringT ConcatStrings(tStringT str1, tStringT str2) {
    tStringT newString;

    // Set length to -1 to indicate failure by default
    newString.length = -1;

    // If concatenated length exceeds limit, return invalid string
    if (StringLength(str1) + StringLength(str2) > 100) {
        return newString;
    }

    int j = 0;
    // Copy first string
    for (int i = 0; i < StringLength(str1); i++) {
        newString.string[j++] = str1.string[i];
    }
    // Copy second string
    for (int i = 0; i < StringLength(str2); i++) {
        newString.string[j++] = str2.string[i];
    }
    newString.length = StringLength(str1) + StringLength(str2);

    // Mark end with '.'
    newString.string[newString.length] = '.';

    return newString;
}

// Displays a tStringT string
void DisplayString(tStringT str) {
    for (int i = 0; i < str.length; i++)
        printf("%c%c", str.string[i], (i == str.length - 1 ? '\n' : '\0'));
    printf("\n");
}

// Returns the length of a tStringT string
int StringLength(tStringT str) {
    return str.length;
}