#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "indexedTable.h"
#include "linkedTable.h"

#define MAX_LINE 5000

typedef int SearchMoveIdx(tLinkedTable *tab, int key);
typedef tData * SearchMoveEnc(tLinkedTable **tab, int key);

void RemoveNewline(char *str){
    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';
}

int LoadFile(const char *filename, tLinkedTable *tabIdx, tLinkedTable **tabEnc){
    FILE *fp;
    char line[MAX_LINE+1], *id, *title, *rating, *votes, *comma, *quote;
    int count;
    tMovie movie;

    fp = fopen(filename, "r");
    if (!fp){
        puts("Error opening file.");
        exit(1);
    }

    count = 0;
    while(1){
        fgets(line, MAX_LINE+1, fp);
        if (feof(fp)){
            break;
        }
        RemoveNewline(line);
        comma = strchr(line, ',');
        if (comma){
            comma[-1] = '\0';
            id = line+1;
            
            title = comma + 2;
            quote = strchr(title, '\"');
            *quote = '\0';
            movie.id = atoi(id);
            movie.title = malloc(sizeof(char) * strlen(title) + 1);
            if (!movie.title){
                printf("Error allocating title for movie #%d\n", count);
            }
            strcpy(movie.title, title);
            
            rating = quote + 3;
            quote = strchr(rating, '\"');
            *quote = '\0';
            movie.averageRating = strtod(rating, NULL);

            votes = quote + 3;
            quote = strchr(votes, '\"');
            *quote = '\0';
            movie.numVotes = strtod(votes, NULL);

            count++;
            //printf("Movie: %s [%s] (id=%s, votes=%s)\n", title, rating, id, votes);
            AddElementIdx(tabIdx, &movie);
            AddElementEnc(tabEnc, &movie);
        }
    }

    fclose(fp);
    return count;
}

int main(void){
    tLinkedTable *idxTab;
    tLinkedTable *linkTab;
    int q, id;
    clock_t start, end;
    double cpuTime;

    idxTab = CreateTableIdx();
    CreateTableEnc(&linkTab);
    start = clock();
    q = LoadFile("../../TMDB_movie_dataset_v11.csv", idxTab, &linkTab);
    end = clock();
    cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Started:\n");
    printf("\t%d movies loaded.\n", q);
    printf("\tIndexed table size: %d\n", LengthIdx(idxTab));
    printf("\tLinked table size: %d\n", LengthEnc(linkTab));
    printf("\tCPU time: %lf\n", cpuTime);
    start = clock();
    Sort(idxTab);
    end = clock();
    cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;    
    printf("\tCPU time for sorting: %lf\n", cpuTime);

    SearchMoveIdx *searchTypeIdx = MoveToFrontSearchLinkedTable();
    SearchMoveEnc *searchTypeEnc = MoveToFrontSearchLinkedTable();

    while(1){
        printf("\nEnter the movie ID to search: ");
        scanf("%d", &id);

        printf("Indexed List:\n");
        start = clock();
        int ind = SequentialSearch(idxTab, id);
        end = clock();
        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tSequential search operations: %d\n", OperationCount());
        printf("\tCPU time: %lf\n", cpuTime);
        
        /*
        start = clock();
        ind = searchTypeIdx(tabIdx, id);
        end = clock();
        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tSearch with move operations: %d\n", OperationCount());
        printf("\tCPU time: %lf\n", cpuTime);
        */

        start = clock();
        ind = BinarySearch(idxTab, id);
        end = clock();
        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tBinary search operations: %d\n", OperationCount());
        printf("\tCPU time: %lf\n", cpuTime);

        start = clock();
        ind = InterpolationSearch(idxTab, id);
        end = clock();
        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tInterpolation search operations: %d\n", OperationCount());
        printf("\tCPU time: %lf\n", cpuTime);

        if (ind < 0){
            puts("\tmovie not found");
        }
        else{
            tMovie *movie = GetElementIdx(idxTab, ind);
            PrintMovie(movie);
        }

        printf("\nLinked List:\n");
        start = clock();
        tMovie * movie = SequentialSearchEnc(linkTab, id);
        end = clock();
        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tLinked search operations: %d\n", OperationCountEnc());
        printf("\tCPU time: %lf\n", cpuTime);
        
        start = clock();
        movie = searchTypeEnc(&linkTab, id);
        end = clock();
        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\tSearch with move operations: %d\n", OperationCountEnc());
        printf("\tCPU time: %lf\n", cpuTime);

        if (!movie){
            puts("\tmovie not found");
        }
        else{
            PrintMovie(movie);
        }
   }

    //DestroyTableEnc(tabEnc);
    DestroyTableIdx(idxTab);
    return 0;
}
