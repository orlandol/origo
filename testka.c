
#include <stdio.h>

#include "keyarray.h"

/*
 *  String Key Array declarations and tests.
 */

  typedef struct FruitData {
    unsigned value;
  } FruitData;

  void CopyFruit( FruitData* destData, FruitData* sourceData ) {
    if( destData && sourceData ) {
      destData->value = sourceData->value;
    }
  }

  void FreeFruit( FruitData* data ) {
  }

  DECLARE_STRING_KEYARRAY_TYPES( FruitList, FruitData )

  DECLARE_STRING_KEYARRAY_CREATE( CreateFruitList, FruitList )
  DECLARE_STRING_KEYARRAY_FREE( FreeFruitList, FruitList, FreeFruit )
  DECLARE_STRING_KEYARRAY_INSERT( InsertFruit, FruitList, FruitData )
  DECLARE_STRING_KEYARRAY_REMOVE( RemoveFruit, FruitList, FreeFruit )
  DECLARE_STRING_KEYARRAY_RETRIEVE( RetrieveFruit, FruitList, FruitData )
  DECLARE_STRING_KEYARRAY_RELEASEUNUSED( ReleaseUnusedFruits, FruitList )
  DECLARE_STRING_KEYARRAY_COPY( CopyFruitList, FruitList, Fruit, CopyFruit, FreeFruit )

  void RunStringKeyArrayTests( void ) {
    FruitList* list;
    FruitList* listCopy;
    FruitData  data;

    list = CreateFruitList(0);

    printf( "\n" );
    printf( "[Testing string key array]\n" );

    data.value = 1234;
    InsertFruit( list, "Orange", &data );

    data.value = 2345;
    InsertFruit( list, "Apple", &data );

    data.value = 3456;
    InsertFruit( list, "Zucchini", &data );

    data.value = 4567;
    InsertFruit( list, "Zucchini", &data );

    data.value = 5678;
    InsertFruit( list, "Apple", &data );

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
    RetrieveFruit( list, "Apple", &data );
    printf( "  Apple: %u\n", data.value );

    data.value = 0;
    RetrieveFruit( list, "Orange", &data );
    printf( "  Orange: %u\n", data.value );

    data.value = 0;
    RetrieveFruit( list, "Zucchini", &data );
    printf( "  Zucchini: %u\n", data.value );
    printf( "\n" );

    ReleaseUnusedFruits( list );

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
    RetrieveFruit( list, "Apple", &data );
    printf( "  Apple: %u\n", data.value );

    data.value = 0;
    RetrieveFruit( list, "Orange", &data );
    printf( "  Orange: %u\n", data.value );

    data.value = 0;
    RetrieveFruit( list, "Zucchini", &data );
    printf( "  Zucchini: %u\n", data.value );

    listCopy = CopyFruitList(list);

    FreeFruitList( &list );

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
    RetrieveFruit( listCopy, "Apple", &data );
    printf( "  Apple: %u\n", data.value );

    data.value = 0;
    RetrieveFruit( listCopy, "Orange", &data );
    printf( "  Orange: %u\n", data.value );

    data.value = 0;
    RetrieveFruit( listCopy, "Zucchini", &data );
    printf( "  Zucchini: %u\n", data.value );

    printf( "\n" );

    printf( "After Remove...\n" );
    RemoveFruit( listCopy, "Apple" );
    RemoveFruit( listCopy, "Zucchini" );
    RemoveFruit( listCopy, "Orange" );

    if( (listCopy == NULL) || (listCopy->item == NULL) ) {
      printf( "  NULL list\n" );
    } else {
      for( unsigned i = 0; i < listCopy->itemCount; i++ ) {
        printf( "  key: %s; value: %u\n",
          listCopy->item[i].key, listCopy->item[i].data.value );
      }
    }
    printf( "\n" );

    FreeFruitList( &listCopy );
  }

/*
 *  Uint Key Array declarations and tests.
 */

  typedef struct FruitIDData {
    unsigned value;
  } FruitIDData;

  void CopyFruitID( FruitIDData* destData, FruitIDData* sourceData ) {
    if( destData && sourceData ) {
      destData->value = sourceData->value;
    }
  }

  void FreeFruitID( FruitIDData* data ) {
  }

  DECLARE_UINT_KEYARRAY_TYPES( FruitIDList, FruitIDData )

  DECLARE_UINT_KEYARRAY_CREATE( CreateFruitIDList, FruitIDList )
  DECLARE_UINT_KEYARRAY_FREE( FreeFruitIDList, FruitIDList, FreeFruit )
  DECLARE_UINT_KEYARRAY_INSERT( InsertFruitID, FruitIDList, FruitIDData )
  DECLARE_UINT_KEYARRAY_REMOVE( RemoveFruitID, FruitIDList, FreeFruit )
  DECLARE_UINT_KEYARRAY_RETRIEVE( RetrieveFruitID, FruitIDList, FruitIDData )
  DECLARE_UINT_KEYARRAY_RELEASEUNUSED( ReleaseUnusedFruitsID, FruitIDList )
  DECLARE_UINT_KEYARRAY_COPY( CopyFruitListID, FruitIDList, Fruit, CopyFruit, FreeFruit )

  void RunUintKeyArrayTests( void ) {
    FruitIDList* list;
    FruitIDList* listCopy;
    FruitIDData data;
  
    list = CreateFruitIDList(0);
  
    printf( "\n" );
    printf( "[Testing uint key array]\n" );

    data.value = 1234;
    InsertFruitID( list, 150, &data );
  
    data.value = 2345;
    InsertFruitID( list, 100, &data );
  
    data.value = 3456;
    InsertFruitID( list, 260, &data );
  
    data.value = 4567;
    InsertFruitID( list, 260, &data );
  
    data.value = 5678;
    InsertFruitID( list, 100, &data );
  
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
    RetrieveFruitID( list, 100, &data );
    printf( "  100: %u\n", data.value );
  
    data.value = 0;
    RetrieveFruitID( list, 150, &data );
    printf( "  150: %u\n", data.value );
  
    data.value = 0;
    RetrieveFruitID( list, 260, &data );
    printf( "  260: %u\n", data.value );
    printf( "\n" );
  
    ReleaseUnusedFruitsID( list );
  
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
    RetrieveFruitID( list, 100, &data );
    printf( "  100: %u\n", data.value );
  
    data.value = 0;
    RetrieveFruitID( list, 150, &data );
    printf( "  150: %u\n", data.value );
  
    data.value = 0;
    RetrieveFruitID( list, 260, &data );
    printf( "  260: %u\n", data.value );
  
    listCopy = CopyFruitListID(list);
  
    FreeFruitIDList( &list );
  
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
    RetrieveFruitID( listCopy, 100, &data );
    printf( "  100: %u\n", data.value );
  
    data.value = 0;
    RetrieveFruitID( listCopy, 150, &data );
    printf( "  150: %u\n", data.value );
  
    data.value = 0;
    RetrieveFruitID( listCopy, 260, &data );
    printf( "  260: %u\n", data.value );
  
    printf( "\n" );
  
    printf( "After Remove...\n" );
    RemoveFruitID( listCopy, 100 );
    RemoveFruitID( listCopy, 260 );
    RemoveFruitID( listCopy, 150 );
  
    if( (listCopy == NULL) || (listCopy->item == NULL) ) {
      printf( "  NULL list\n" );
    } else {
      for( unsigned i = 0; i < listCopy->itemCount; i++ ) {
        printf( "  key: %u; value: %u\n",
          listCopy->item[i].key, listCopy->item[i].data.value );
      }
    }
    printf( "\n" );

    FreeFruitIDList( &listCopy );
  }

/*
 *  Main program
 */

int main( int argc, char* argv[] ) {

  RunStringKeyArrayTests();

  RunUintKeyArrayTests();

  return 0;
}
