
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

  typedef struct UintKeyArrayData {
    unsigned value;
  } UintKeyArrayData;

  typedef struct UintKeyArrayItem {
    unsigned key;
    UintKeyArrayData data;
  } UintKeyArrayItem;

  typedef struct UintKeyArray {
    size_t reservedCount;
    size_t itemCount;
    UintKeyArrayItem* item;
  } UintKeyArray;

  void FreeUintKeyArrayData( UintKeyArrayData* keyData );

  UintKeyArray* CreateUintKeyArray( size_t reserveCount ) {
    UintKeyArray* newKeyArray = NULL;

    newKeyArray = (UintKeyArray*)calloc(1, sizeof(UintKeyArray));
    if( newKeyArray == NULL ) {
      goto ReturnError;
    }

    if( reserveCount ) {
      newKeyArray->item =
        (UintKeyArrayItem*)calloc(reserveCount, sizeof(UintKeyArrayItem));
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

  typedef void (*FreeUintKeyArrayDataFunc)( UintKeyArrayData* keyData );

  void FreeUintKeyArray( UintKeyArray** keyList, FreeUintKeyArrayDataFunc freeData) {
    size_t index;
    size_t itemCount;

    if( keyList && (*keyList) ) {
      if( freeData ) {
        itemCount = (*keyList)->itemCount;
        for( index = 0; index < itemCount; index++ ) {
          freeData( &((*keyList)->item[index].data) );
        }
      }

      free( (*keyList) );
      (*keyList) = NULL;
    }
  }

  bool InsertUintKeyArrayItem( UintKeyArray* keyList,
      unsigned key, UintKeyArrayData* data ) {
    unsigned leftIndex;
    unsigned insertIndex;
    unsigned rightIndex;
    unsigned prevCount;
    unsigned reservedCount;
    unsigned itemCount;
    UintKeyArrayItem* item;

    if( !(keyList && data) ) {
      return false;
    }

    // Grow list, if necessary
    reservedCount = keyList->reservedCount;
    itemCount = keyList->itemCount;
    item = keyList->item;

    if( itemCount == reservedCount ) {
      prevCount = reservedCount;
      reservedCount += 8;
      if( prevCount > reservedCount ) {
        return false;
      }

      item = realloc(item, reservedCount * sizeof(UintKeyArrayItem));
      if( item == NULL ) {
        return false;
      }
      keyList->reservedCount = reservedCount;
      keyList->item = item;
    }

    // Search for insert position
    leftIndex = 0;
    rightIndex = itemCount;
    insertIndex = itemCount / 2;

    while( leftIndex < rightIndex ) {
      if( item[insertIndex].key == key ) {
        return false;
      }

      if( item[insertIndex].key > key ) {
        rightIndex = insertIndex;
      } else {
        leftIndex = insertIndex + 1;
      }
  
      insertIndex = (leftIndex + rightIndex) / 2;
    }

    // Move data past insertion point up, if necessary
    memmove( &(item[insertIndex + 1]), &(item[insertIndex]),
        (itemCount - insertIndex) * sizeof(UintKeyArrayItem) );

    // Insert item
    item[insertIndex].key = key;
    if( data ) {
      memcpy( &(item[insertIndex].data), data, sizeof(UintKeyArrayData) );
    }

    keyList->itemCount++;

    return true;
  }

  void RemoveUintKeyArrayItem( UintKeyArray* keyList, unsigned key ) {
    unsigned leftIndex;
    unsigned rightIndex;
    unsigned removeIndex;
    unsigned reservedCount;
    unsigned itemCount;
    UintKeyArrayItem* item;

    if( !(keyList && keyList->item) ) {
      return;
    }

    reservedCount = keyList->reservedCount;
    itemCount = keyList->itemCount;
    item = keyList->item;

    // Search for insert position
    leftIndex = 0;
    rightIndex = itemCount;
    removeIndex = itemCount / 2;

    while( leftIndex < rightIndex ) {
      if( item[removeIndex].key == key ) {
        FreeUintKeyArrayData( &(item[removeIndex].data) );

        if( itemCount ) {
          itemCount--;

          memcpy( &(item[removeIndex]), &(item[removeIndex + 1]),
            (itemCount - removeIndex) * sizeof(UintKeyArrayItem) );

          keyList->itemCount = itemCount;
        }

        memset( &(item[itemCount]), 0, sizeof(UintKeyArrayItem) );

        return;
      }

      if( item[removeIndex].key > key ) {
        rightIndex = removeIndex;
      } else {
        leftIndex = removeIndex + 1;
      }
  
      removeIndex = (leftIndex + rightIndex) / 2;
    }
  }

  bool RetrieveUintKeyArrayData( UintKeyArray* keyList, unsigned key,
      UintKeyArrayData* destData ) {
    unsigned leftIndex;
    unsigned rightIndex;
    unsigned retrieveIndex;
    unsigned reservedCount;
    unsigned itemCount;
    UintKeyArrayItem* item;

    if( !(keyList && keyList->item && destData) ) {
      return false;
    }

    reservedCount = keyList->reservedCount;
    itemCount = keyList->itemCount;
    item = keyList->item;

    // Search for insert position
    leftIndex = 0;
    rightIndex = itemCount;
    retrieveIndex = itemCount / 2;

    while( leftIndex < rightIndex ) {
      if( item[retrieveIndex].key == key ) {
        memcpy( destData, &(item[retrieveIndex].data), sizeof(UintKeyArrayData) );
        return true;
      }

      if( item[retrieveIndex].key > key ) {
        rightIndex = retrieveIndex;
      } else {
        leftIndex = retrieveIndex + 1;
      }
  
      retrieveIndex = (leftIndex + rightIndex) / 2;
    }

    return false;
  }

  void ReleaseUnusedUintKeyArrayItems( UintKeyArray* keyList ) {
    UintKeyArrayItem* item;

    if( keyList == NULL ) {
      return;
    }

    if( keyList->item && keyList->itemCount ) {
      // Resize to remove reserved space
      item = realloc(keyList->item,
        keyList->itemCount * sizeof(UintKeyArrayItem));
      if( item ) {
        keyList->item = item;
        keyList->reservedCount = keyList->itemCount;
      }
    } else {
      // Deallocate
      keyList->reservedCount = 0;
      keyList->itemCount = 0;
      if( keyList->item ) {
        free( keyList->item );
        keyList->item = NULL;
      }
    }
  }

  typedef void (*CopyUintKeyDataFunc)( UintKeyArrayData* destData,
      UintKeyArrayData* sourceData );

  void CopyUintKeyData( UintKeyArrayData* destData,
      UintKeyArrayData* srcData );

  UintKeyArray* CopyUintKeyArray( UintKeyArray* sourceList ) {
    UintKeyArray* newCopy = NULL;
    UintKeyArrayItem* sourceItem = NULL;
    size_t reservedCount = 0;
    size_t itemCount = 0;
    size_t index;

    if( sourceList == NULL ) {
      return NULL;
    }

    // Attempt to allocate list object
    newCopy = calloc(1, sizeof(UintKeyArray));
    if( newCopy == NULL ) {
      goto ReturnError;
    }

    // Initialize important variables
    reservedCount = sourceList->reservedCount;
    itemCount = sourceList->itemCount;
    sourceItem = sourceList->item;

    // A list with no items is valid
    if( !(reservedCount && itemCount && sourceItem) ) {
      return newCopy;
    }

    // Copy data, then copy the Uint keys
    newCopy->item = malloc(reservedCount * sizeof(UintKeyArrayItem));
    if( newCopy->item == NULL ) {
      goto ReturnError;
    }

    for( index = 0; index < itemCount; index++ ) {
      CopyUintKeyData( &(newCopy->item[index].data),
          &(sourceItem[index].data) );

      newCopy->item[index].key = sourceItem[index].key;
    }

    newCopy->reservedCount = reservedCount;
    newCopy->itemCount = itemCount;

    return newCopy;

  ReturnError:
    if( newCopy == NULL ) {
      return NULL;
    }

    if( newCopy->item ) {
      for( index = 0; index < itemCount; index++ ) {
        FreeUintKeyArrayData( &(newCopy->item[index].data) );
      }
    }

    free( newCopy );
    newCopy = NULL;

    return NULL;
  }

  void CopyUintKeyData( UintKeyArrayData* destData,
      UintKeyArrayData* srcData ) {
    if( destData && srcData ) {
      destData->value = srcData->value;
    }
  }

  void FreeUintKeyArrayData( UintKeyArrayData* keyData ) {
    if( keyData ) {
      keyData->value = 0;
    }
  }

int main( int argc, char* argv[] ) {
  UintKeyArray* list = NULL;
  UintKeyArray* listCopy = NULL;
  UintKeyArrayData data;

  list = CreateUintKeyArray(0);

  data.value = 1234;
  InsertUintKeyArrayItem( list, 150, &data );

  data.value = 2345;
  InsertUintKeyArrayItem( list, 100, &data );

  data.value = 3456;
  InsertUintKeyArrayItem( list, 260, &data );

  data.value = 4567;
  InsertUintKeyArrayItem( list, 260, &data );

  data.value = 5678;
  InsertUintKeyArrayItem( list, 100, &data );

  printf( "\n" );
  printf( "Before Copy, and ReleaseUnused...\n" );
  if( (list == NULL) || (list->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < list->itemCount; i++ ) {
      printf( "  key: %u; value: %u\n",
        list->item[i].key, list->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveUintKeyArrayData( list, 100, &data );
  printf( "  100: %u\n", data.value );

  data.value = 0;
  RetrieveUintKeyArrayData( list, 150, &data );
  printf( "  150: %u\n", data.value );

  data.value = 0;
  RetrieveUintKeyArrayData( list, 260, &data );
  printf( "  260: %u\n", data.value );
  printf( "\n" );

  ReleaseUnusedUintKeyArrayItems( list );

  printf( "After ReleaseUnused...\n" );
  if( (list == NULL) || (list->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < list->itemCount; i++ ) {
      printf( "  key: %u; value: %u\n",
        list->item[i].key, list->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveUintKeyArrayData( list, 100, &data );
  printf( "  100: %u\n", data.value );

  data.value = 0;
  RetrieveUintKeyArrayData( list, 150, &data );
  printf( "  150: %u\n", data.value );

  data.value = 0;
  RetrieveUintKeyArrayData( list, 260, &data );
  printf( "  260: %u\n", data.value );

  listCopy = CopyUintKeyArray(list);

  FreeUintKeyArray( &list, FreeUintKeyArrayData );

  printf( "\n" );

  printf( "After Copy...\n" );
  if( (listCopy == NULL) || (listCopy->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < listCopy->itemCount; i++ ) {
      printf( "  key: %u; value: %u\n",
        listCopy->item[i].key, listCopy->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveUintKeyArrayData( listCopy, 100, &data );
  printf( "  100: %u\n", data.value );

  data.value = 0;
  RetrieveUintKeyArrayData( listCopy, 150, &data );
  printf( "  150: %u\n", data.value );

  data.value = 0;
  RetrieveUintKeyArrayData( listCopy, 260, &data );
  printf( "  260: %u\n", data.value );

  printf( "\n" );

  printf( "After Remove...\n" );
  RemoveUintKeyArrayItem( listCopy, 100 );
  RemoveUintKeyArrayItem( listCopy, 260 );
  RemoveUintKeyArrayItem( listCopy, 150 );

  if( (listCopy == NULL) || (listCopy->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < listCopy->itemCount; i++ ) {
      printf( "  key: %u; value: %u\n",
        listCopy->item[i].key, listCopy->item[i].data.value );
    }
  }
  printf( "\n" );

  FreeUintKeyArray( &listCopy, FreeUintKeyArrayData );

  return 0;
}
