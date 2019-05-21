
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

  typedef struct _KeyItem {
    char*  name;
    size_t dataSize;
    uint8_t* data;
  } KeyItem;

  typedef struct _KeyList {
    size_t itemCount;
    KeyItem* item;
  } KeyList;

  KeyList* CreateKeyList( size_t reserveCount ) {
    return NULL;
  }

  void FreeKeyList( KeyList** keyList ) {
  }

int main( int argc, char* argv[] ) {
  KeyList* list;

  list = CreateKeyList(0);

  FreeKeyList( &list );

  return 0;
}
