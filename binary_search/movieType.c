#include <stdio.h>
#include <stdlib.h>
#include "movieType.h"

void PrintMovie(tMovie *movie) {
    printf("\tMovie: %s [%.1f] (id=%d, votes=%d)\n", movie->title, movie->averageRating, movie->id, movie->numVotes);
}

void FreeMovie(tMovie *movie) {
    free(movie->title);
}