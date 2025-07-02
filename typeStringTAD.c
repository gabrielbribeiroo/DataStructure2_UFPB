#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tipoStringTAD.h" // You may want to rename this to stringTAD.h for consistency

// Definition of the stringTAD structure
struct stringTAD {
    char string[100];    // Character array to store the string
    int length;          // Length of the string
};

// Creates a new stringTAD from a C string
tStringTAD CreateString(const char *str) {
    tStringTAD newString = malloc(sizeof(struct stringTAD));

    if (!newString) {
        return NULL;
    }

    newString->length = -1;
    strcpy(newString->string, str);
    newString->length = strlen(str);

    return newString;
}

// Destroys a stringTAD and frees its memory
void DestroyString(tStringTAD *s) {
    free(*s);
    *s = NULL;
}

// Concatenates two stringTADs and returns a new stringTAD
tStringTAD ConcatStrings(tStringTAD str1, tStringTAD str2) {
    tStringTAD newString = CreateString("");
    newString->length = -1; // Indicates that the copy was not successful

    if (StringLength(str1) + StringLength(str2) > 100) {
        return newString;
    }

    int j = 0;
    for (int i = 0; i < StringLength(str1); i++) {
        newString->string[j++] = str1->string[i];
    }
    for (int i = 0; i < StringLength(str2); i++) {
        newString->string[j++] = str2->string[i];
    }
    newString->length = StringLength(str1) + StringLength(str2);
    newString->string[newString->length] = '.'; // Forces it to not be a C string

    return newString;
}

// Displays the stringTAD
void ShowString(tStringTAD str) {
    for (int i = 0; i < str->length; i++) {
        printf("%c%c", str->string[i], (i == str->length - 1 ? '\n' : '\0'));
    }
}

// Returns the length of the stringTAD
int StringLength(tStringTAD str) {
    return str->length;
}