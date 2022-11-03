
#include <stdio.h>

#include "lists.h"


/*
 *  List declarations
 */

typedef unsigned LISTITEM_TYPENAME;

typedef struct LIST_TYPENAME {
  LISTITEM_TYPENAME* item;
  unsigned reserveCount;
  unsigned itemCount;
} LIST_TYPENAME;

LIST_TYPENAME* CreateList();
void ReleaseList( LIST_TYPENAME** listPtr );

unsigned ReleaseListItems( LIST_TYPENAME* list,
  unsigned (*releaseItem)(LISTITEM_TYPENAME* itemPtr) );

unsigned GrowList( LIST_TYPENAME* list );
unsigned CompactList( LIST_TYPENAME* list );

unsigned InsertItem( LIST_TYPENAME* list, LISTITEM_TYPENAME sourceItem );
unsigned InsertItemAt( LIST_TYPENAME* list, unsigned index, LISTITEM_TYPENAME sourceItem );

unsigned RemoveItemAt( LIST_TYPENAME* list, unsigned index, LISTITEM_TYPENAME* destItem );

unsigned ReadItem( LIST_TYPENAME* list, unsigned atIndex, LISTITEM_TYPENAME* destItem );
unsigned WriteItem( LIST_TYPENAME* list, unsigned atIndex, const LISTITEM_TYPENAME* sourceItem );

unsigned FindItem( LIST_TYPENAME* list, unsigned byValue, LISTITEM_TYPENAME* destItem );

/*
 *  Main program
 */

LIST_TYPENAME* list = NULL;

void Cleanup() {
  ReleaseList( &list );
}

void DumpList( LIST_TYPENAME* sourceList, unsigned marker ) {
  unsigned itemIndex = 0;

  printf( "Dumping list %u - list(%p) item(%p) reserveCount(%u) itemCount(%u)\n",
      marker, list, list ? list->item : NULL, list ? list->reserveCount : 0,
      list ? list->itemCount : 0 );

  if( (sourceList == NULL) || (sourceList->itemCount == 0) ) {
    printf( "  List empty.\n" );
    goto Done;
  }

  for( itemIndex = 0; itemIndex < list->itemCount; itemIndex++ ) {
    printf( "  List->item[%u] == %u\n",
      itemIndex, list->item[itemIndex] );
  }

Done:
  printf(
    "...Done.\n"
    "\n"
  );
}

int main( int argc, char** argv ) {
  unsigned result;

  atexit( Cleanup );

  list = CreateList();

  DumpList( list, 0 );

  result = CompactList(list);
  if( result ) {
    printf( "Error in CompactList[%u]: list(%p) item(%p) reserveCount(%u) itemCount(%u)\n",
      result, list, list ? list->item: NULL, list ? list->reserveCount : 0,
      list ? list->itemCount : 0 );
    exit(1);
  }

  DumpList( list, 1 );

  result = InsertItem(list, 1234);
  if( result ) {
    printf( "Error in InsertItem[%u]: list(%p) items(%p) reserveCount(%u) itemCount(%u)\n",
      result, list, list ? list->item: NULL, list ? list->reserveCount : 0,
      list ? list->itemCount : 0 );
    exit(1);
  }

  DumpList( list, 2 );

  result = CompactList(list);
  if( result ) {
    printf( "Error in CompactList[%u]: list(%p) item(%p) reserveCount(%u) itemCount(%u)\n",
      result, list, list ? list->item: NULL, list ? list->reserveCount : 0,
      list ? list->itemCount : 0 );
    exit(1);
  }

  DumpList( list, 3 );

  result = InsertItem(list, 2345);
  if( result ) {
    printf( "Error in InsertItem[%u]: list(%p) items(%p) reserveCount(%u) itemCount(%u)\n",
      result, list, list ? list->item: NULL, list ? list->reserveCount : 0,
      list ? list->itemCount : 0 );
    exit(1);
  }

  DumpList( list, 4 );

  result = CompactList(list);
  if( result ) {
    printf( "Error in CompactList[%u]: list(%p) item(%p) reserveCount(%u) itemCount(%u)\n",
      result, list, list ? list->item: NULL, list ? list->reserveCount : 0,
      list ? list->itemCount : 0 );
    exit(1);
  }

  DumpList( list, 5 );

  Cleanup();

  return 0;
}


/*
 *  List implementation
 */

#ifndef LIST_GROWTH_NUMBITS
  #define LIST_GROWTH_NUMBITS (4)
#endif
#define LIST_GROWTH_INCREMENT (1 << LIST_GROWTH_NUMBITS)
#define LIST_GROWTH_MASK (LIST_GROWTH_INCREMENT - 1)

LIST_TYPENAME* CreateList() {
  return calloc(1, sizeof(LIST_TYPENAME));
}

void ReleaseList( LIST_TYPENAME** listPtr ) {
  if( listPtr ) {
    if( (*listPtr) ) {
      if( (*listPtr)->item ) {
        free( (*listPtr)->item );
        (*listPtr)->item = NULL;
      }

      free( (*listPtr) );
      (*listPtr) = NULL;
    }
  }
}

unsigned ReleaseListItems( LIST_TYPENAME* list,
    unsigned (*releaseItem)(LISTITEM_TYPENAME* itemPtr) ) {

  unsigned index;

  if( list == NULL ) { return 1; }
  if( releaseItem == NULL ) { return 2; }

  if( list->item ) {
    for( index = 0; index < list->itemCount; index++ ) {
      if( releaseItem(&list->item[index]) ) {
        return 3;
      }
    }
  }

  return 0;
}

unsigned GrowList( LIST_TYPENAME* list ) {
  LISTITEM_TYPENAME* newItem = NULL;
  unsigned newReserveCount = 0;

  if( list == NULL ) { return 1; }

  if( (((unsigned)-1) - list->reserveCount) < LIST_GROWTH_INCREMENT ) {
    return 2;
  }

  newReserveCount = (list->reserveCount & (~LIST_GROWTH_MASK)) + LIST_GROWTH_INCREMENT;

  if( list->itemCount == list->reserveCount ) {
    newItem = realloc(list->item, newReserveCount * sizeof(LISTITEM_TYPENAME));
    if( newItem == NULL ) { return 3; }

    list->item = newItem;
    list->reserveCount = newReserveCount;

    return 0;
  }

  return 0;
}

/*
#define IMPLEMENT_COMPACTSTACK( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack ) {\
  STACKSLOT_TYPENAME* newSlots = NULL;\
  unsigned newTop;\
  unsigned newBottom;\
  \
  if( (stack == NULL) || (stack->top < stack->bottom) ) { return 1; }\
  \
  newTop = stack->top - stack->bottom;\
  newBottom = 0;\
  \
  if( newTop > newBottom ) {\
    if( stack->slot ) {\
      memmove( &stack->slot[0], &stack->slot[stack->bottom],\
        stack->bottom * sizeof(STACKSLOT_TYPENAME) );\
    }\
    \
    newSlots = realloc(stack->slot, newTop * sizeof(STACKSLOT_TYPENAME));\
    if( newSlots == NULL ) {\
      return 2;\
    }\
    \
    stack->slot = newSlots;\
    stack->top = newTop;\
    stack->bottom = newBottom;\
    \
    return 0;\
  }\
  \
  if( stack->slot ) {\
    free( stack->slot );\
    stack->slot = NULL;\
  }\
  \
  stack->top = 0;\
  stack->bottom = 0;\
  \
  return 0;\
}
*/

unsigned CompactList( LIST_TYPENAME* list ) {
  LISTITEM_TYPENAME* tmpItems = NULL;
  unsigned newReserveCount;

  if( (list == NULL) ||
      (list->reserveCount < list->itemCount) ) {
    return 1;
  }

  newReserveCount = list->itemCount;

  if( list->reserveCount > list->itemCount ) {
    tmpItems = realloc(list->item,newReserveCount *
        sizeof(LISTITEM_TYPENAME));
    if( tmpItems == NULL ) {
      return 2;
    }

    list->item = tmpItems;
    list->reserveCount = newReserveCount;

    return 0;
  }

  if( list->item ) {
    free( list->item );
    list->item = NULL;
  }

  list->reserveCount = 0;
  list->itemCount = 0;

  return 0;
}

unsigned InsertItem( LIST_TYPENAME* list, LISTITEM_TYPENAME sourceItem ) {
  if( list == NULL ) { return 1; }

  if( GrowList(list) ) {
    return 3;
  }

  if( list->item == NULL ) { return 4; }

  list->item[list->itemCount++] = sourceItem;

  return 0;
}

unsigned InsertItemAt( LIST_TYPENAME* list, unsigned index, LISTITEM_TYPENAME sourceItem ) {
  return 4;
}

unsigned RemoveItemAt( LIST_TYPENAME* list, unsigned index, LISTITEM_TYPENAME* destItem ) {
  return 4;
}

unsigned ListItem( LIST_TYPENAME* list, unsigned atIndex, LISTITEM_TYPENAME* destItem ) {
  return 4;
}

unsigned FindItem( LIST_TYPENAME* list, unsigned byValue, LISTITEM_TYPENAME* destItem ) {
  return 4;
}
