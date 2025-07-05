#ifndef _INDEXEDTABLE_H_
#define _INDEXEDTABLE_H_

#include "movieType.h"

typedef tMovie tData;
typedef struct indexedTableNode tIndexedTable;

extern void CreateIndexedTable(tIndexedTable **table);
extern void DestroyIndexedTable(tIndexedTable *table);
extern int IndexedTableLength(tIndexedTable *table);
extern tData * RemoveIndexedElement(tIndexedTable **table, int id);
extern int IsIndexedTableEmpty(tIndexedTable *table);
extern void AddIndexedElement(tIndexedTable **table, const tData *element);
extern tData * SequentialIndexedSearch(tIndexedTable *table, int key);
extern tData * JumpIndexedSearch(tIndexedTable *table, int key);
extern tData * TransposeIndexedSearch(tIndexedTable **table, int key);
extern tData * MoveToFrontIndexedSearch(tIndexedTable **table, int key);
extern int IndexedOperationCount();
#endif