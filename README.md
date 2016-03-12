# Yet Another Hash Table

Implementation consists of several files:

  - [yetanotherht.c]: This is an example that shows how to use hash table. You do not necessarily need it.
  - [yaht.c]: Contains Hash_Create() and Hash_Destroy(..) functions. Compile this file and link with your code.
  - [yaht.h]: Basic defines and data types. Include in your source file.
  - [yaht.inl.h]: This file only contains static *inline* functions. Include in your source file. Keep in mind that this will *increase* code size.

## Platforms
Code has been tested on Debian and MS Windows (Cygwin & MSVC).

For now it only supports 32/64 bit architectures. If you want to use it on another systems which have different integer size, for instance 8 or 16 bit, you need to add [FNV Hash] basis and prime parameters for that specific architecture.

If you use another compiler than GCC or MSVC, functions may not be inlined. See *FORCEINLINE* macro in *yaht.inl.h*.

### Makefile

Targets:
* all: Compile example program.
* test: Compile example program and see how much memory is allocated and freed.

### Visual Studio

Open project file and compile.

## Example Code

Run it, type some text/words and see how array size is increased and collisions or duplicate items are detected.

When you compile test target, calloc(..) is replaced by wrapper function and at the end you will see how much memory is allocated and freed.

##### Why *calloc(..)*?
Well, when you need null values *calloc(..)* is your friend. Instead of setting them manually *calloc(..)* is called. It is also used in example code to make it easier to see used memory where I could use *malloc(..)*.

Simple test:
```sh
$ make test
$ ls /usr/bin/ | wc -l
2009
$ ls /usr/bin/ | ./yetanotherht
Bits: 2, array size: 4
Doubled the size of hash table: 8 (/3)
Doubled the size of hash table: 16 (/6)
Doubled the size of hash table: 32 (/11)
Doubled the size of hash table: 64 (/22)
Doubled the size of hash table: 128 (/43)
Doubled the size of hash table: 256 (/86)
Doubled the size of hash table: 512 (/171)
Doubled the size of hash table: 1024 (/342)
Doubled the size of hash table: 2048 (/683)
Doubled the size of hash table: 4096 (/1366)
Item count: 2009
Total freed: 150317 bytes of 150317 bytes
```

## API

As mentioned earlier some functions are forcibly inlined to avoid function call overhead when working with high amount of insertion or deletions. Of course, this may  lead to performance degradation and CPU cache problems when large functions are forcibly inlined.

### Functions
There are create, insert, lookup, destroy functions.

- - -
```c
PHashTable Hash_Create()
```
Returns a pointer to HashTable. Pointer will be used for lookups, insertions and deletions.
- - -
```c
int Hash_HashFNV(char * data, unsigned int len)
```
This will return a hash value for any type of data with given length. You may use it in several hash functions.
- - -
```c
char * Hash_LookupStr(PHashTable pHT, char * str)
```
Do lookup using a C string. Returns a pointer to corresponding item or NULL when key is not found.
- - -
```c
void * Hash_Lookup(PHashTable pHT, unsigned int hash)
```
Do lookup using a *hash* value which can also be calculated by any hash function or provided FNV hash function.
- - -
```c
void Hash_AddStrKey(PHashTable pHT, char * strKey, void * item)
```
Adds an item (a pointer) to the given hash table and uses C string as key value.
- - -
```c
void Hash_Add(PHashTable pHT, unsigned int hash, void * item);
```
Adds an item to the given hash table and given hash value must be calculated by *some* hash function.
- - -
```c
void Hash_Destroy(PHashTable pHT)
```
Destroys given hash table.  
**NOTE:** This will not touch any user data. Therefore, you need to free items that were added to hash table before destroying it.
- - -
```c
  PHashItem pItem;
  for (pItem = pHT->pListHead; pItem; pItem = pItem->pNext)
    puts(pItem->pItem); //assuming pItem is a string.
```
Traverse **all** items in hash table pHT and print each value.
- - -
### Data Types

* PHashTable: Pointer to hash table. It is used for hash operations.
* PHashItem: Pointer to hash item. This holds next and previous items in bucket and in whole hash table. User data is also accessible with HashItem through *pItem* field.

## TODOs
* Add delete function.
* Pre-allocated memory blocks can be used for hash items which will avoid memory fragmentation.
* Add few more user friendly APIs.

## License
[Unlicense]

   [yaht.c]: <yaht.c>
   [yaht.h]: <yaht.h>
   [yaht.inl.h]: <yaht.inl.h>
   [yetanotherht.c]: <yetanotherht.c>
   [FNV Hash]: <http://www.isthe.com/chongo/tech/comp/fnv/>
   [Unlicense]: <http://unlicense.org/>

