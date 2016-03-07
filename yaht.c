#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef CALLOC_TEST
  #include <unistd.h>
#endif

#include "yaht.h"

#define HASH_DEF_BITS 2
#define HASH_DEF_ARSZ (1 << (HASH_DEF_BITS))

#ifdef CALLOC_TEST
size_t dbg_fragsize = 0;
unsigned int totalFree = 0;
#endif

#ifndef SWAIT
#define SWAIT 2
#endif

PHashTable Hash_Create()
{
  PHashTable pHT = calloc(1, sizeof(HashTable));
  if (!pHT)
    return NULL;
  pHT->ppItems = calloc(HASH_DEF_ARSZ, sizeof(PHashItem)); //array of pointers
  if (!pHT->ppItems)
  {
    free(pHT);
    return NULL;
  }

  pHT->bits = HASH_DEF_BITS;
  pHT->size = HASH_DEF_ARSZ;
  SET_THRESHOLD(pHT);
  fprintf(stderr, "Bits: %d, array size: %d\r\n", HASH_DEF_BITS, HASH_DEF_ARSZ);
  return pHT;
}

void Hash_Destroy(PHashTable pHT)
{
  PHashItem pHI;
  //Delete all hash items
  //NOTE: This WILL NOT free pItem (pointer to user data)
#ifdef CALLOC_TEST
  sleep(SWAIT);
#endif
  PHashItem next;
  for (pHI = pHT->pListHead; pHI; pHI = next)
  {
    next = pHI->pNext;
#ifdef CALLOC_TEST
    //also free pItem that contains a string
    totalFree += sizeof(HashItem) + (strlen(pHI->pItem) + 1); free(pHI->pItem);
#endif
    free(pHI);
  }

#ifdef CALLOC_TEST
  totalFree += sizeof(PHashItem) * pHT->size;
#endif

  free(pHT->ppItems); //free pointer array
#ifdef CALLOC_TEST
  totalFree += sizeof(HashTable);
#endif
  free(pHT);
#ifdef CALLOC_TEST
  fprintf(stderr, "Total freed: %u bytes of %u bytes\r\n", totalFree, dbg_fragsize);
  sleep(SWAIT);
#endif
}