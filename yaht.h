#ifndef yaht_h__
#define yaht_h__

#include <stdlib.h> //calloc

#define SET_THRESHOLD(pHT) (pHT->threshold = (pHT->size * 2) / 3)

/*********************************************************/
// STRUCTURES
/*********************************************************/
typedef struct tagHashItem
{
  void * pItem;
  struct tagHashItem * pBucketNext;
  struct tagHashItem * pNext;
  unsigned int hash;
}HashItem, *PHashItem;

typedef struct tagHashTable
{
  PHashItem * ppItems; //array of pointers
  PHashItem pListHead;
  unsigned int bits; // size in bits, 5 bits = 32
  unsigned int size;
  unsigned int count;
  unsigned int threshold;
}HashTable, *PHashTable;

/*********************************************************/
// PROTOTYPES
/*********************************************************/
PHashTable Hash_Create();
void Hash_Destroy(PHashTable pHT);

/*********************************************************/
// TEST DEFINES/FUNCTIONS/VARIABLES
/*********************************************************/
#ifdef CALLOC_TEST

extern size_t dbg_fragsize;
extern unsigned int totalFree;

static void * dbg_calloc(size_t c, size_t s)
{
  dbg_fragsize += c*s;
  return calloc(c, s);
}

#define calloc dbg_calloc

#endif //CALLOC_TEST


#endif // yaht_h__