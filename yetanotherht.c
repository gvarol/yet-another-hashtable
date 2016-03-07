#include <stdio.h>
#include <string.h>

#include "yaht.h"
#include "yaht.inl.h"

int main(int argc, char* argv[]);

//Test function
char * strrev(char *str)
{
  int i = strlen(str) - 1, j = 0;
  char ch;
  while (i > j)
  {
    ch = str[i];
    str[i] = str[j];
    str[j] = ch;
    i--;
    j++;
  }
  return str;
}

static char * mystrdup(const char * str)
{
  char * tmp;
  tmp = calloc(strlen(str) + 1, 1); //calloc/debug
  strcpy(tmp, str);
  return tmp;
}

int main(int argc, char* argv[])
{
  FILE * pfWords = stdin; //fopen("words.txt", "rb");
  PHashTable pHT = Hash_Create();
  char line[2048];

  while (fgets(line, sizeof(line), pfWords))
  {
    if (strlen(line))
      line[strlen(line) - 1] = '\0';

    {
      char * pTmp;
      //Collision check
      if ( (pTmp = Hash_LookupStr(pHT, line)) )
      {
        printf("Collision found!\r\n->\t\"%s\"\r\n->\t\"%s\"\r\n", pTmp, line);
      }
      else
      {
        char * dup = strrev(mystrdup(line));
        Hash_AddStrKey(pHT, line, dup);
      }
    }
  }

  fprintf(stderr, "Item count: %d\r\n", pHT->count);

  if (argc > 1)
    fprintf(stderr, "Lookup item: %s -> %s\r\n", argv[1], Hash_LookupStr(pHT, argv[1]));
  if (argc > 2)
  {
    PHashItem pItem;
    for (pItem = pHT->pListHead; pItem; pItem = pItem->pNext)
      puts(pItem->pItem);
  }
  Hash_Destroy(pHT);
  fclose(pfWords);
  return 0;
}
