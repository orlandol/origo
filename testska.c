
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

  typedef struct StringKeyArrayData {
    unsigned value;
  } StringKeyArrayData;

  typedef struct StringKeyArrayItem {
    char* key;
    StringKeyArrayData data;
  } StringKeyArrayItem;

  typedef struct StringKeyArray {
    size_t reservedCount;
    size_t itemCount;
    StringKeyArrayItem* item;
  } StringKeyArray;

  void FreeStringKeyArrayData( StringKeyArrayData* keyData );

  StringKeyArray* CreateStringKeyArray( size_t reserveCount ) {
    StringKeyArray* newKeyArray = NULL;

    newKeyArray = (StringKeyArray*)calloc(1, sizeof(StringKeyArray));
    if( newKeyArray == NULL ) {
      goto ReturnError;
    }

    if( reserveCount ) {
      newKeyArray->item =
        (StringKeyArrayItem*)calloc(reserveCount, sizeof(StringKeyArrayItem));
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

  typedef void (*FreeStringKeyArrayDataFunc)( StringKeyArrayData* keyData );

  void FreeStringKeyArray( StringKeyArray** keyList, FreeStringKeyArrayDataFunc freeItem ) {
    size_t index;
    size_t itemCount;

    if( keyList && (*keyList) ) {
      if( freeItem ) {
        itemCount = (*keyList)->itemCount;
        for( index = 0; index < itemCount; index++ ) {
          if( (*keyList)->item[index].key ) {
            free( (*keyList)->item[index].key );
          }
          freeItem( &((*keyList)->item[index].data) );
        }
      }

      free( (*keyList) );
      (*keyList) = NULL;
    }
  }

  bool InsertStringKeyArrayItem( StringKeyArray* keyList,
      char* key, StringKeyArrayData* data ) {
    unsigned leftIndex;
    unsigned insertIndex;
    unsigned rightIndex;
    int result;
    char* newStrKey;
    size_t keyLen;
    unsigned prevCount;
    unsigned reservedCount;
    unsigned itemCount;
    StringKeyArrayItem* item;

    if( !(keyList && key && data) ) {
      return false;
    }

    keyLen = strlen(key);
    if( keyLen == 0 ) {
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

      item = realloc(item, reservedCount * sizeof(StringKeyArrayItem));
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
      result = strcmp(item[insertIndex].key, key);

      if( result == 0 ) {
        return false;
      }

      if( result > 0 ) {
        rightIndex = insertIndex;
      } else {
        leftIndex = insertIndex + 1;
      }
  
      insertIndex = (leftIndex + rightIndex) / 2;
  }

    // Attempt to allocate key string before going further
    newStrKey = malloc(keyLen + 1);
    if( newStrKey == NULL ) {
      return false;
    }
    strcpy( newStrKey, key );

    // Move data past insertion point up, if necessary
    memmove( &(item[insertIndex + 1]), &(item[insertIndex]),
        (itemCount - insertIndex) * sizeof(StringKeyArrayItem) );

    // Insert item
    item[insertIndex].key = newStrKey;
    if( data ) {
      memcpy( &(item[insertIndex].data), data, sizeof(StringKeyArrayData) );
    }

    keyList->itemCount++;

    return true;
  }

  void RemoveStringKeyArrayItem( StringKeyArray* keyList, char* key ) {
    unsigned leftIndex;
    unsigned rightIndex;
    unsigned removeIndex;
    int result;
    unsigned reservedCount;
    unsigned itemCount;
    StringKeyArrayItem* item;

    if( !(keyList && keyList->item && key && (*key)) ) {
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
      result = strcmp(item[removeIndex].key, key);

      if( result == 0 ) {
        FreeStringKeyArrayData( &(item[removeIndex].data) );
        if( item[removeIndex].key ) {
          free( item[removeIndex].key );
          item[removeIndex].key = NULL;
        }

        if( itemCount ) {
          itemCount--;

          memcpy( &(item[removeIndex]), &(item[removeIndex + 1]),
            (itemCount - removeIndex) * sizeof(StringKeyArrayItem) );

          keyList->itemCount = itemCount;
        }

        memset( &(item[itemCount]), 0, sizeof(StringKeyArrayItem) );

        return;
      }

      if( result > 0 ) {
        rightIndex = removeIndex;
      } else {
        leftIndex = removeIndex + 1;
      }
  
      removeIndex = (leftIndex + rightIndex) / 2;
    }
  }

  bool RetrieveStringKeyArrayData( StringKeyArray* keyList, char* key,
      StringKeyArrayData* destData ) {
    unsigned leftIndex;
    unsigned rightIndex;
    unsigned retrieveIndex;
    int result;
    unsigned reservedCount;
    unsigned itemCount;
    StringKeyArrayItem* item;

    if( !(keyList && keyList->item && key && (*key) && destData) ) {
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
      result = strcmp(item[retrieveIndex].key, key);

      if( result == 0 ) {
        memcpy( destData, &(item[retrieveIndex].data), sizeof(StringKeyArrayData) );
        return true;
      }

      if( result > 0 ) {
        rightIndex = retrieveIndex;
      } else {
        leftIndex = retrieveIndex + 1;
      }
  
      retrieveIndex = (leftIndex + rightIndex) / 2;
    }

    return false;
  }

  void ReleaseUnusedStringKeyArrayItems( StringKeyArray* keyList ) {
    StringKeyArrayItem* item;

    if( keyList == NULL ) {
      return;
    }

    if( keyList->item && keyList->itemCount ) {
      // Resize to remove reserved space
      item = realloc(keyList->item,
        keyList->itemCount * sizeof(StringKeyArrayItem));
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

  typedef void (*CopyStringKeyDataFunc)( StringKeyArrayData* destData,
      StringKeyArrayData* sourceData );

  void CopyStringKeyData( StringKeyArrayData* destData,
      StringKeyArrayData* srcData );

  StringKeyArray* CopyStringKeyArray( StringKeyArray* sourceList ) {
    StringKeyArray* newCopy = NULL;
    StringKeyArrayItem* sourceItem = NULL;
    size_t reservedCount = 0;
    size_t itemCount = 0;
    char* keyCopy;
    size_t keyLen;
    size_t index;

    if( sourceList == NULL ) {
      return NULL;
    }

    // Attempt to allocate list object
    newCopy = calloc(1, sizeof(StringKeyArray));
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

    // Copy data, then copy the string keys
    newCopy->item = malloc(reservedCount * sizeof(StringKeyArrayItem));
    if( newCopy->item == NULL ) {
      goto ReturnError;
    }

    for( index = 0; index < itemCount; index++ ) {
      CopyStringKeyData( &(newCopy->item[index].data),
          &(sourceItem[index].data) );

      keyLen = strlen(sourceItem[index].key);
      keyCopy = malloc(keyLen + 1);
      if( keyCopy == NULL ) {
        goto ReturnError;
      }
      strcpy( keyCopy, sourceItem[index].key );

      newCopy->item[index].key = keyCopy;
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
        FreeStringKeyArrayData( &(newCopy->item[index].data) );

        keyCopy = newCopy->item[index].key;
        if( keyCopy ) {
          free( keyCopy );
          keyCopy = NULL;
        }
      }
    }

    free( newCopy );
    newCopy = NULL;

    return NULL;
  }

  void CopyStringKeyData( StringKeyArrayData* destData,
      StringKeyArrayData* srcData ) {
    if( destData && srcData ) {
      destData->value = srcData->value;
    }
  }

  void FreeStringKeyArrayData( StringKeyArrayData* keyData ) {
    if( keyData ) {
      keyData->value = 0;
    }
  }

int main( int argc, char* argv[] ) {
  StringKeyArray* list = NULL;
  StringKeyArray* listCopy = NULL;
  StringKeyArrayData data;

  list = CreateStringKeyArray(0);

  data.value = 1234;
  InsertStringKeyArrayItem( list, "Orange", &data );

  data.value = 2345;
  InsertStringKeyArrayItem( list, "Apple", &data );

  data.value = 3456;
  InsertStringKeyArrayItem( list, "Zucchini", &data );

  data.value = 4567;
  InsertStringKeyArrayItem( list, "Zucchini", &data );

  data.value = 5678;
  InsertStringKeyArrayItem( list, "Apple", &data );

  printf( "\n" );
  printf( "Before Copy, and ReleaseUnused...\n" );
  if( (list == NULL) || (list->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < list->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        list->item[i].key, list->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveStringKeyArrayData( list, "Apple", &data );
  printf( "  Apple: %u\n", data.value );

  data.value = 0;
  RetrieveStringKeyArrayData( list, "Orange", &data );
  printf( "  Orange: %u\n", data.value );

  data.value = 0;
  RetrieveStringKeyArrayData( list, "Zucchini", &data );
  printf( "  Zucchini: %u\n", data.value );
  printf( "\n" );

  ReleaseUnusedStringKeyArrayItems( list );

  printf( "After ReleaseUnused...\n" );
  if( (list == NULL) || (list->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < list->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        list->item[i].key, list->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveStringKeyArrayData( list, "Apple", &data );
  printf( "  Apple: %u\n", data.value );

  data.value = 0;
  RetrieveStringKeyArrayData( list, "Orange", &data );
  printf( "  Orange: %u\n", data.value );

  data.value = 0;
  RetrieveStringKeyArrayData( list, "Zucchini", &data );
  printf( "  Zucchini: %u\n", data.value );

  listCopy = CopyStringKeyArray(list);

  FreeStringKeyArray( &list, FreeStringKeyArrayData );

  printf( "\n" );

  printf( "After Copy...\n" );
  if( (listCopy == NULL) || (listCopy->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < listCopy->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        listCopy->item[i].key, listCopy->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveStringKeyArrayData( listCopy, "Apple", &data );
  printf( "  Apple: %u\n", data.value );

  data.value = 0;
  RetrieveStringKeyArrayData( listCopy, "Orange", &data );
  printf( "  Orange: %u\n", data.value );

  data.value = 0;
  RetrieveStringKeyArrayData( listCopy, "Zucchini", &data );
  printf( "  Zucchini: %u\n", data.value );

  printf( "\n" );

  printf( "After Remove...\n" );
  RemoveStringKeyArrayItem( listCopy, "Apple" );
  RemoveStringKeyArrayItem( listCopy, "Zucchini" );
  RemoveStringKeyArrayItem( listCopy, "Orange" );

  if( (listCopy == NULL) || (listCopy->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < listCopy->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        listCopy->item[i].key, listCopy->item[i].data.value );
    }
  }
  printf( "\n" );

  FreeStringKeyArray( &listCopy, FreeStringKeyArrayData );

  return 0;
}
