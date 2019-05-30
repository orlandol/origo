
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

  typedef int (*CompareStringKeyFunc)( const char* left, const char* right );

  bool InsertStringKeyArrayItem( StringKeyArray* keyList, CompareStringKeyFunc compareKeys,
      char* key, StringKeyArrayData* data ) {
    unsigned leftIndex;
    unsigned insertIndex;
    unsigned rightIndex;
    int result;
    char* newStrKey;
    size_t keyLen;
    unsigned reservedCount;
    unsigned itemCount;
    StringKeyArrayItem* item;

    if( !(keyList && compareKeys && key && data) ) {
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
      reservedCount += 8;
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

    while( leftIndex > rightIndex ) {
      result = compareKeys(item[insertIndex].key, key);

      if( result == 0 ) {
        return false;
      }

      if( result < 0 ) {
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
printf( "insertIndex: %u; (itemCount - insertIndex): %u\n", insertIndex, (itemCount - insertIndex) );
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
    return;
  ReturnError:
    return;
  }

  bool RetrieveStringKeyArrayData( StringKeyArray* keyList, char* key,
      StringKeyArrayData* destData ) {
    return false;
  ReturnError:
    return false;
  }

  void FreeStringKeyArrayData( StringKeyArrayData* keyData ) {
    if( keyData ) {
      keyData->value = 0;
    }
  }

int main( int argc, char* argv[] ) {
  StringKeyArray* list = NULL;
  StringKeyArrayData data;

  list = CreateStringKeyArray(0);

  data.value = 1234;
  InsertStringKeyArrayItem( list, strcmp, "Orange", &data );

  data.value = 2345;
  InsertStringKeyArrayItem( list, strcmp, "Apple", &data );

  data.value = 3456;
  InsertStringKeyArrayItem( list, strcmp, "Zucchini", &data );

  for( unsigned i = 0; i < list->itemCount; i++ ) {
    printf( "key: %s; value: %u\n", list->item[i].key, list->item[i].data.value );
  }

  FreeStringKeyArray( &list, FreeStringKeyArrayData );

  return 0;
}
