#include <stdio.h>
#include "tipoStringT.h"

// Main function
int main(void) {
    tStringT oneString, anotherString;

    // Create a string with "cafe"
    oneString = CriaString("cafe");
    ExibeString(oneString); // Display the string
    printf("length: %d\n", ComprimentoString(oneString)); // Print its length

    // Create another string with "Amargo"
    anotherString = CriaString("Amargo");
    // Concatenate the two strings and display the result
    ExibeString(ConcatenaStrings(oneString, anotherString));
    // Print the length of the concatenated string
    printf("length: %d\n", ComprimentoString(ConcatenaStrings(oneString, anotherString)));

    return 0;
}