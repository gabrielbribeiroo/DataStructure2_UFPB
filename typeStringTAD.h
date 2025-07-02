#ifndef _TYPE_STRING_TAD_H_
#define _TYPE_STRING_TAD_H_

// Incomplete type definition for the stringTAD structure
typedef struct stringTAD *tStringTAD;

/**
 * @brief Creates a new tStringTAD from a C-style string.
 * 
 * @param str The C-style string to initialize the tStringTAD.
 * @return A new tStringTAD object.
 */
extern tStringTAD CreateString(const char *str);

/**
 * @brief Destroys a tStringTAD object and frees its memory.
 * 
 * @param c Pointer to the tStringTAD object to destroy.
 */
extern void DestroyString(tStringTAD *c);

/**
 * @brief Concatenates two tStringTAD strings.
 *
 * This function takes two tStringTAD objects as input and returns a new tStringTAD
 * that is the result of concatenating str1 and str2.
 *
 * @param str1 The first string to concatenate.
 * @param str2 The second string to concatenate.
 * @return A new tStringTAD containing the concatenation of str1 and str2.
 */
extern tStringTAD ConcatStrings(tStringTAD str1, tStringTAD str2);

/**
 * @brief Displays the contents of a tStringTAD.
 * 
 * @param str The tStringTAD object to display.
 */
extern void ShowString(tStringTAD str);

/**
 * @brief Returns the length of a tStringTAD string.
 * 
 * @param str The tStringTAD object.
 * @return The length of the string.
 */
extern int StringLength(tStringTAD str);

#endif