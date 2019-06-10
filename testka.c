
#include <stdio.h>

#include "keyarray.h"

  typedef struct FruitData {
    unsigned value;
  } FruitData;

  DECLARE_STRING_KEYARRAY_TYPES( FruitList, FruitData )

  DECLARE_STRING_KEYARRAY_CREATE( CreateFruitList, FruitList )
  DECLARE_STRING_KEYARRAY_FREE( FreeFruitList, FruitList, FreeFruit )
  DECLARE_STRING_KEYARRAY_INSERT( InsertFruit, FruitList, FruitData )
  DECLARE_STRING_KEYARRAY_REMOVE( RemoveFruit, FruitList, FreeFruit )
  DECLARE_STRING_KEYARRAY_RETRIEVE( RetrieveFruit, FruitList, FruitData )
  DECLARE_STRING_KEYARRAY_RELEASEUNUSED( ReleaseUnusedFruits, FruitList )
  DECLARE_STRING_KEYARRAY_COPY( CopyFruitList, FruitList, Fruit, CopyFruit, FreeFruit )

  void CopyFruit( FruitData* destData, FruitData* sourceData ) {
  }

  void FreeFruit( FruitData* data ) {
  }

int main( int argc, char* argv[] ) {
  FruitList* fruitList;
  FruitList* fruitListCopy;
  FruitData  data;

  fruitList = CreateFruitList(0);

  data.value = 1234;
  InsertFruit( fruitList, "Orange", &data );

  data.value = 2345;
  InsertFruit( fruitList, "Apple", &data );

  data.value = 3456;
  InsertFruit( fruitList, "Zucchini", &data );

  data.value = 4567;
  InsertFruit( fruitList, "Zucchini", &data );

  data.value = 5678;
  InsertFruit( fruitList, "Apple", &data );

  printf( "\n" );
  printf( "Before Copy, and ReleaseUnused...\n" );
  if( (fruitList == NULL) || (fruitList->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < fruitList->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        fruitList->item[i].key, fruitList->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveFruit( fruitList, "Apple", &data );
  printf( "  Apple: %u\n", data.value );

  data.value = 0;
  RetrieveFruit( fruitList, "Orange", &data );
  printf( "  Orange: %u\n", data.value );

  data.value = 0;
  RetrieveFruit( fruitList, "Zucchini", &data );
  printf( "  Zucchini: %u\n", data.value );
  printf( "\n" );

  ReleaseUnusedFruits( fruitList );

  printf( "After ReleaseUnused...\n" );
  if( (fruitList == NULL) || (fruitList->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < fruitList->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        fruitList->item[i].key, fruitList->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveFruit( fruitList, "Apple", &data );
  printf( "  Apple: %u\n", data.value );

  data.value = 0;
  RetrieveFruit( fruitList, "Orange", &data );
  printf( "  Orange: %u\n", data.value );

  data.value = 0;
  RetrieveFruit( fruitList, "Zucchini", &data );
  printf( "  Zucchini: %u\n", data.value );

  fruitListCopy = CopyStringKeyArray(fruitList);

  FreeStringKeyArray( &fruitList, FreeStringKeyArrayData );

  printf( "\n" );

  printf( "After Copy...\n" );
  if( (fruitListCopy == NULL) || (fruitListCopy->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < fruitListCopy->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        fruitListCopy->item[i].key, fruitListCopy->item[i].data.value );
    }
  }
  printf( "\n" );

  data.value = 0;
  RetrieveFruit( fruitListCopy, "Apple", &data );
  printf( "  Apple: %u\n", data.value );

  data.value = 0;
  RetrieveFruit( fruitListCopy, "Orange", &data );
  printf( "  Orange: %u\n", data.value );

  data.value = 0;
  RetrieveFruit( fruitListCopy, "Zucchini", &data );
  printf( "  Zucchini: %u\n", data.value );

  printf( "\n" );

  printf( "After Remove...\n" );
  RemoveFruit( fruitListCopy, "Apple" );
  RemoveFruit( fruitListCopy, "Zucchini" );
  RemoveFruit( fruitListCopy, "Orange" );

  if( (fruitListCopy == NULL) || (fruitListCopy->item == NULL) ) {
    printf( "  NULL list\n" );
  } else {
    for( unsigned i = 0; i < fruitListCopy->itemCount; i++ ) {
      printf( "  key: %s; value: %u\n",
        fruitListCopy->item[i].key, fruitListCopy->item[i].data.value );
    }
  }
  printf( "\n" );

  FreeFruitList( &fruitList );

  return 0;
}
