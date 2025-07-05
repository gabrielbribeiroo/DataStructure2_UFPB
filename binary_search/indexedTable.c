#include <stdio.h>
#include <stdlib.h>
#include "linkedTable.h"
#include "movieType.h"

struct linkedTableNode {
    tData data;
    struct linkedTableNode *next;
};

static int nOperations = 0;

void CreateLinkedTable(tLinkedTable **table){
    *table = NULL;
}

void DestroyLinkedTable(tLinkedTable *table){
    tLinkedTable *temp;
    while(table != NULL){
        temp = table;
        table = table->next;

        FreeMovie(&temp->data);
        free(temp);
    }
}

int LengthLinkedTable(tLinkedTable *table){
    int c = 0;

    while(table != NULL){
        c++;
        table = table->next;
    }

    return c;
}

tData * RemoveElementLinkedTable(tLinkedTable **table, int id){
    tLinkedTable *prev = NULL, *curr = *table, *removed = NULL;
    while(curr != NULL){
        if (curr->data.id == id){
            // element found
            removed = curr;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (removed != NULL){
        if (prev != NULL){
            prev->next = removed->next;
        }else{
            // first element removed
            (*table)->next = removed->next;
        }
        tData *ret = malloc(sizeof(tData));
        if (!ret){
            puts("Error allocating memory for return element.");
            exit(1);
        }
        *ret = removed->data;
        free(removed);
        return ret;
    }

    return NULL;
}

int IsEmptyLinkedTable(tLinkedTable *table){
    return table == NULL;
}

void AddElementLinkedTable(tLinkedTable **table, const tData *element){
    tLinkedTable *newNode = malloc(sizeof(tLinkedTable));
    if (!newNode){
        puts("Error allocating memory for new element.");
        exit(1);
    }
    newNode->data = *element;
    newNode->next = *table;
    *table = newNode;
}

tData * SequentialSearchLinkedTable(tLinkedTable *table, int id){
    nOperations = 0;
    while(table != NULL){
        nOperations++;
        if (table->data.id == id){
            return &table->data;
        }
        table = table->next;
    }

    return NULL;
}

tData * FingerSearchLinkedTable(tLinkedTable *table, int id){
    static tLinkedTable *lastNode = NULL;
    if (lastNode == NULL){
        lastNode = table;
    }

    nOperations = 0;
    while(lastNode != NULL){
        if (lastNode->data.id == id){
            return &lastNode->data;
        }
        lastNode = lastNode->next;
    }

    return NULL;
}

tData * TranspositionSearchLinkedTable(tLinkedTable **table, int key){
    tLinkedTable *prev1 = NULL, *prev2 = NULL, *curr = *table;
    nOperations = 0;
    while(curr != NULL){
        nOperations++;
        if (curr->data.id == key){
            if (prev1 != NULL){
                prev1->next = curr->next;
                curr->next = prev1;
                if (prev2 == NULL){
                    // curr becomes head
                    *table = curr;
                }else{
                    prev2->next = curr;
                }
            }else{
                // already first
            }
            return &curr->data;
        }
        prev2 = prev1;
        prev1 = curr;
        curr = curr->next;
    }

    return NULL;
}

tData * MoveToFrontSearchLinkedTable(tLinkedTable **table, int key){
    tLinkedTable *prev = NULL, *curr = *table;
    nOperations = 0;
    while(curr != NULL){
        nOperations++;
        if (curr->data.id == key){
            if (prev != NULL){
                prev->next = curr->next;
                curr->next = *table;
                *table = curr;
            }else{
                // already first
            }
            return &curr->data;
        }
        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

int NumberOfOperationsLinkedTable(){
    return nOperations;
}