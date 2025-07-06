#ifndef _MOVIETYPE_H_
#define _MOVIETYPE_H_

typedef struct movieStruct {
    int id;                 // primary key
    char *title;            // must be allocated
    float averageRating;
    int numVotes;
} tMovie;

extern void PrintMovie(tMovie *movie);
extern void FreeMovie(tMovie *movie);

#endif