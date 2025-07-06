#ifndef _LINKEDTABLE_H_
#define _LINKEDTABLE_H_

#include "movieType.h"

typedef tMovie tData;
typedef struct linkedTableNode tLinkedTable;

extern void CreateLinkedTable(tLinkedTable **table);
extern void DestroyLinkedTable(tLinkedTable *table);
extern int LinkedTableLength(tLinkedTable *table);
extern tData * RemoveLinkedElement(tLinkedTable **table, int id);
extern int IsLinkedTableEmpty(tLinkedTable *table);
extern void AddLinkedElement(tLinkedTable **table, const tData *element);
extern tData * SequentialLinkedSearch(tLinkedTable *table, int key);
extern tData * FingerLinkedSearch(tLinkedTable *table, int key);
extern tData * TranspositionLinkedSearch(tLinkedTable **table, int key);
extern tData * MoveToFrontLinkedSearch(tLinkedTable **table, int key);
extern int LinkedTableOperationCount();
#endif