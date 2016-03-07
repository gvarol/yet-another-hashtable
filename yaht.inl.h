#include <stdint.h>
#include <limits.h>

#include "yaht.h"

#ifdef _MSC_VER
#define FORCEINLINE __forceinline
#else
#ifdef __GNUC__
#define FORCEINLINE inline __attribute__((always_inline))
#else
#error "Unsupported compiler"
#endif
#endif

/*********************************************************/
// CODE DEFINES
/*********************************************************/
#define FNV_BASIS64 0xcbf29ce484222325 //14695981039346656037
#define FNV_PRIME64 0x100000001b3      //1099511628211
#define FNV_BASIS32 0x811c9dc5         //2166136261
#define FNV_PRIME32 0x1000193          //16777619

#define HASH_STR(str) Hash_HashFNV((str), strlen(str))

/*********************************************************/
// PROTOTYPES
/*********************************************************/
FORCEINLINE static int Hash_HashFNV(char * data, unsigned int len);
FORCEINLINE static void Hash_Add(PHashTable pHT, unsigned int hash, void * item);
FORCEINLINE static void Hash_AddStrKey(PHashTable pHT, char * strKey, void * item);
FORCEINLINE static void * Hash_Lookup(PHashTable pHT, unsigned int hash);
FORCEINLINE static char * Hash_LookupStr(PHashTable pHT, char * str);

/*********************************************************/
// BODY
/*********************************************************/
FORCEINLINE static int Hash_HashFNV(char * data, unsigned int len)
{
#if defined(UINT_MAX) && defined(UINT32_MAX) && defined(UINT64_MAX)
#else
#error "Int types are not defined"
#endif

#define ARCH32 (UINT_MAX == UINT32_MAX)
#define ARCH64 (UINT_MAX == UINT64_MAX)

  unsigned int i, hash;
#if ARCH32
  hash = FNV_BASIS32;
#elif ARCH64
  hash = FNV_BASIS64;
#else
#error "Unsupported architecture"
#endif
  for (i = 0; i < len; i++)
  {
    hash ^= data[i];
#if ARCH32
    hash *= FNV_PRIME32;
#elif ARCH64
    hash *= FNV_PRIME64;
#endif
  }

  return hash;
}

FORCEINLINE static void Hash_Add(PHashTable pHT, unsigned int hash, void * item)
{
  if (pHT->count > pHT->threshold) // Recreate array after threshold
  {
    PHashItem * ppNewArray = calloc(pHT->size * 2, sizeof(PHashItem));
    if (ppNewArray)
    {
      int idx;
      PHashItem pHI;
      free(pHT->ppItems);
#ifdef CALLOC_TEST
      totalFree += pHT->size * sizeof(PHashItem);
#endif
      pHT->ppItems = ppNewArray;
      pHT->size *= 2;
      for (pHI = pHT->pListHead; pHI; pHI = pHI->pNext)
      {
        idx = pHI->hash % pHT->size;
        if (ppNewArray[idx])
          pHI->pBucketNext = ppNewArray[idx]; //old head of the bucket
        else
          pHI->pBucketNext = NULL; //not in the same bucket anymore

        ppNewArray[idx] = pHI; //new head for the bucket
      }
      SET_THRESHOLD(pHT);
      fprintf(stderr, "Doubled the size of hash table: %u (/%u)\r\n", pHT->size, pHT->count);
    }
  }

  int idx = hash % pHT->size;
  if (pHT->ppItems[idx])
  {
    PHashItem pPrev = pHT->ppItems[idx];
    for (;;)
    {
      if (pPrev->hash == hash)
      {
        if (pPrev->pItem != item)
        {
          //TODO: Need to handle collisions, give option 
        }
        pPrev->pItem = item; //Overwrites!
        return;
      }
      if (!pPrev->pBucketNext)
        break;
      pPrev = pPrev->pBucketNext;
    }

    PHashItem pHI = calloc(1, sizeof(HashItem));
    pHI->pItem = item;
    pHI->hash = hash;
    pHI->pNext = pHT->pListHead;
    pHT->pListHead = pHI;
    pPrev->pBucketNext = pHI;
  }
  else
  {
    PHashItem pHI = calloc(1, sizeof(HashItem));
    pHI->pItem = item;
    pHI->hash = hash;
    pHI->pNext = pHT->pListHead;
    pHT->pListHead = pHI;
    pHT->ppItems[idx] = pHI;
  }
  pHT->count++;
}

FORCEINLINE static void Hash_AddStrKey(PHashTable pHT, char * strKey, void * item)
{
  Hash_Add(pHT, HASH_STR(strKey), item);
}

FORCEINLINE static void * Hash_Lookup(PHashTable pHT, unsigned int hash)
{
  PHashItem pHI;
  int idx = hash % pHT->size;
  for (pHI = pHT->ppItems[idx]; pHI; pHI = pHI->pBucketNext)
  {
    if (pHI->hash == hash)
      return pHI->pItem;
  }
  return NULL;
}

FORCEINLINE static char * Hash_LookupStr(PHashTable pHT, char * str)
{
  return Hash_Lookup(pHT, HASH_STR(str));
}

