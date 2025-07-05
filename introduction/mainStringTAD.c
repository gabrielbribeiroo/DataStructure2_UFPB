/*
 * mainStringTAD.c
 *
 * This program demonstrates the usage of the tStringTAD abstract data type.
 * It performs the following operations:
 *  - Creates two tStringTAD strings with initial values "cafe" and "Amargo".
 *  - Displays the contents of the first string and prints its length.
 *  - Concatenates the two strings, displays the result, and prints the length of the concatenated string.
 *  - Destroys both string objects to free allocated memory.
 *
 * Functions used:
 *  - CriaString: Creates a new tStringTAD from a C string.
 *  - ExibeString: Displays the contents of a tStringTAD.
 *  - ComprimentoString: Returns the length of a tStringTAD.
 *  - ConcatenaStrings: Concatenates two tStringTAD objects and returns the result.
 *  - DestroiString: Frees the memory allocated for a tStringTAD.
 *
 * Author: [Gabriel Ribeiro]
 * Date: [07-02-2025]
 */
#include <stdio.h>
#include "tipoStringTAD.h"

int main(void) {
    tStringTAD oneString, anotherString;

    oneString = CriaString("coffee");
    ExibeString(oneString);
    printf("length: %d\n", ComprimentoString(oneString));

    anotherString = CriaString("Bitter");
    ExibeString(ConcatenaStrings(oneString, anotherString));
    printf("length: %d\n", ComprimentoString(ConcatenaStrings(oneString, anotherString)));

    DestroiString(&oneString);
    DestroiString(&anotherString);
    
    return 0;
}