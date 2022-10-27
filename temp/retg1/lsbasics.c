
#include <stdio.h>

#include "lists.h"


/*
 *  List declarations
 */

typedef unsigned LISTITEM_TYPENAME;

typedef struct LIST_TYPENAME {
  LISTITEM_TYPENAME item;
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

int main( int argc, char** argv ) {
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
}

unsigned ReleaseListItems( LIST_TYPENAME* list,
    unsigned (*releaseItem)(LISTITEM_TYPENAME* itemPtr) ) {
  return 3;
}

/*
#define IMPLEMENT_GROWSTACK( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack ) {\
  STACKSLOT_TYPENAME* newSlot = NULL;\
  unsigned newTop;\
  unsigned newBottom;\
  \
  if( stack == NULL ) { return 1; }\
  \
  if( (((unsigned)-1) - stack->top) < STACK_GROWTH_INCREMENT ) {\
    return 2;\
  }\
  \
  newTop = (stack->top & (~STACK_GROWTH_MASK)) + STACK_GROWTH_INCREMENT;\
  newBottom = newTop - (stack->top - stack->bottom);\
  \
  if( stack->bottom == 0 ) {\
    newSlot = realloc(stack->slot, newTop * sizeof(STACKSLOT_TYPENAME));\
    if( newSlot == NULL ) {\
      return 3;\
    }\
    \
    if( stack->slot ) {\
      memmove( &newSlot[newBottom], &newSlot[stack->bottom],\
        stack->bottom * sizeof(STACKSLOT_TYPENAME) );\
      memset( newSlot, 0, stack->bottom * sizeof(STACKSLOT_TYPENAME) );\
    }\
    \
    stack->slot = newSlot;\
    stack->top = newTop;\
    stack->bottom = newBottom;\
  }\
  \
  return 0;\
}
*/

unsigned GrowList( LIST_TYPENAME* list ) {
  LISTITEM_TYPENAME* newItem = NULL;
  unsigned newReserveCount = 0;

  if( list == NULL ) { return 1; }

  if( (((unsigned)-1) - list->reserveCount) < LIST_GROWTH_INCREMENT ) {
    return 2;
  }

  newReserveCount = (list->reserveCount & (~LIST_GROWTH_MASK)) + LIST_GROWTH_INCREMENT;

  return 3;
}

unsigned CompactList( LIST_TYPENAME* list ) {
  return 2;
}

unsigned InsertItem( LIST_TYPENAME* list, LISTITEM_TYPENAME sourceItem ) {
  return 3;
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
