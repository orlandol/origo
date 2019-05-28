
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

  typedef char* KeyType;
  typedef struct KeyArrayData {
    unsigned value;
  } KeyArrayData;

  typedef struct KeyArrayItem {
    KeyType key;
    KeyArrayData data;
  } KeyArrayItem;

  typedef struct KeyArray {
    size_t reservedCount;
    size_t itemCount;
    KeyArrayItem* item;
  } KeyArray;

  KeyArray* CreateKeyArray( size_t reserveCount ) {
    KeyArray* newKeyArray = NULL;

    newKeyArray = (KeyArray*)calloc(1, sizeof(KeyArray));
    if( newKeyArray == NULL ) {
      goto ReturnError;
    }

    if( reserveCount ) {
      newKeyArray->item =
        (KeyArrayItem*)calloc(reserveCount, sizeof(KeyArrayItem));
      if( newKeyArray->item == NULL ) {
        goto ReturnError;
      }

      newKeyArray->reservedCount = reserveCount;
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

  typedef void (*FreeKeyArrayItemFunc)( KeyArrayItem* keyItem );

  void FreeKeyArray( KeyArray** keyList, FreeKeyArrayItemFunc freeItem ) {
    size_t index;
    size_t itemCount;

    if( keyList && (*keyList) ) {
      if( freeItem ) {
        itemCount = (*keyList)->itemCount;
        for( index = 0; index < itemCount; index++ ) {
          freeItem( &((*keyList)->item[index]) );
        }
      }

      free( (*keyList) );
      (*keyList) = NULL;
    }
  }

  bool InsertKeyArrayItem( KeyArray* keyList, KeyArrayItem* item ) {
    return false;
  ReturnError:
    return false;
  }

  void RemoveKeyArrayItem( KeyArray* keyList, KeyType key ) {
    return false;
  ReturnError:
    return false;
  }

  bool RetrieveKeyArrayData( KeyArray* keyList, KeyType key,
    KeyArrayData* destData ) {
    return false;
  ReturnError:
    return false;
  }

  void FreeKeyArrayItem( KeyArrayItem* keyItem ) {
    if( keyItem ) {
      keyItem->data.value = 0;
    }
  }

int main( int argc, char* argv[] ) {
  KeyArray* list = NULL;

  list = CreateKeyArray(0);

  FreeKeyArray( &list, FreeKeyArrayItem );

  return 0;
}
