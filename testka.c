
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

  typedef char* KeyType;
  typedef struct KeyItemData {
    unsigned value;
  } KeyItemData;

  typedef struct KeyArrayItem {
    KeyType key;
    KeyItemData data;
  } KeyArrayItem;

  typedef struct KeyArray {
    size_t reservedCount;
    size_t itemCount;
    KeyArrayItem* item;
  } KeyArray;

  KeyArray* CreateKeyArray( size_t reserveCount ) {
    KeyArray* newKeyArray = NULL;
    size_t adjustedCount = 0;

    newKeyArray = (KeyArray*)calloc(1, sizeof(KeyArray));
    if( newKeyArray == NULL ) {
      goto ReturnError;
    }

    if( reserveCount ) {
      adjustedCount = (reserveCount + 7) & (~7);

      newKeyArray->item =
        (KeyArrayItem*)calloc(adjustedCount, sizeof(KeyArrayItem));
      if( newKeyArray->item == NULL ) {
        goto ReturnError;
      }

      newKeyArray->reservedCount = adjustedCount;
    }

    return newKeyArray;

  ReturnError:
    if( newKeyArray ) {
      if( newKeyArray->item ) {
        free( newKeyArray->item );
        newKeyArray->item = NULL;
      }
      free( newKeyArray );
      newKeyArray = NULL;
    }

    return NULL;
  }

  void FreeKeyArrayItem( KeyArrayItem** keyItem ) {
  }

  void FreeKeyArray( KeyArray** keyList ) {
    if( keyList && (*keyList) ) {
      
    }
  }

  bool InsertKeyArrayItem( KeyArray* keyList, KeyArrayItem* item ) {
    return false;
  }


int main( int argc, char* argv[] ) {
  KeyArray* list = NULL;

  list = CreateKeyArray(0);

  return 0;
}
