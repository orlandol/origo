
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

unsigned InsertItem( LIST_TYPENAME* list, LISTITEM_TYPENAME fromItem );

unsigned RemoveItem( LIST_TYPENAME* list, unsigned atIndex, LISTITEM_TYPENAME* toItem );

unsigned ListItem( LIST_TYPENAME* list, unsigned atIndex, LISTITEM_TYPENAME* toItem );

unsigned FindItem( LIST_TYPENAME* list, unsigned byValue, LISTITEM_TYPENAME* toItem );

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

unsigned GrowList( LIST_TYPENAME* list ) {
  return 2;
}

unsigned CompactList( LIST_TYPENAME* list ) {
  return 2;
}

unsigned InsertItem( LIST_TYPENAME* list, LISTITEM_TYPENAME fromItem ) {
  return 3;
}

unsigned RemoveItem( LIST_TYPENAME* list, unsigned atIndex, LISTITEM_TYPENAME* toItem ) {
  return 4;
}

unsigned ListItem( LIST_TYPENAME* list, unsigned atIndex, LISTITEM_TYPENAME* toItem ) {
  return 4;
}

unsigned FindItem( LIST_TYPENAME* list, unsigned byValue, LISTITEM_TYPENAME* toItem ) {
  return 4;
}
