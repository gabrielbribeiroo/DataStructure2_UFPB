/*
 * typeStringT.h
 *
 * This header defines the tStringT structure and related string manipulation functions.
 *
 * Structure:
 *   tStringT
 *     - char string[100]: Array to store the string (maximum 99 characters + null terminator).
 *     - int length: Stores the length of the string.
 *
 * Functions:
 *   tStringT CreateString(const char *str);
 *     - Creates a tStringT from a C-style string.
 *
 *   tStringT ConcatStrings(tStringT str1, tStringT str2);
 *     - Concatenates two tStringT strings and returns the result.
 *
 *   void DisplayString(tStringT str);
 *     - Displays the contents of a tStringT string.
 *
 *   int StringLength(tStringT str);
 *     - Returns the length of the tStringT string.
 */
#ifndef _TypeStringT_H_
#define _TypeStringT_H_

typedef struct {
    char string[100];
    int length;
} tStringT;

extern tStringT CreateString(const char *str);
extern tStringT ConcatStrings(tStringT str1, tStringT str2);
extern void DisplayString(tStringT str);
extern int StringLength(tStringT str);

#endif