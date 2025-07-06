#ifndef _MOVIETYPE_H_
#define _MOVIETYPE_H_

typedef struct movieStruct {
    int id;                 // primary key
    char *title;            // must be allocated
    float averageRating;
    int voteCount;
} Movie;

extern void PrintMovie(Movie *movie);
extern void FreeMovie(Movie *movie);

#endif