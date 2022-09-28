
#include <stdio.h>

#include "lists.h"


/*
 *  Queue declarations
 */

typedef unsigned QUEUESLOT_TYPENAME;

typedef struct QUEUE_TYPENAME {
  QUEUESLOT_TYPENAME* slot;
  unsigned first;
  unsigned last;
  unsigned slotCount;
} QUEUE_TYPENAME;

QUEUE_TYPENAME* CreateQueue();
void ReleaseQueue( QUEUE_TYPENAME** queuePtr );

unsigned ReleaseQueueSlots( QUEUE_TYPENAME* queue,
  unsigned (*releaseSlot)(QUEUESLOT_TYPENAME* slotPtr) );

unsigned GrowQueue( QUEUE_TYPENAME* queue );
unsigned CompactQueue( QUEUE_TYPENAME* queue );

unsigned PushNext( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME fromItem );
unsigned PushLast( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME fromItem );

unsigned PopNext( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem );
unsigned PopLast( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem );

unsigned PeekNext( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem );
unsigned PeekNextAhead( QUEUE_TYPENAME* queue, unsigned byAmount,
  QUEUESLOT_TYPENAME* toItem );

unsigned PeekLast( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem );
unsigned PeekLastFrom( QUEUE_TYPENAME* queue, unsigned amount,
  QUEUESLOT_TYPENAME* toItem );


/*
 *  Main program
 */

int main( int argc, char** argv ) {
  return 0;
}


/*
 *  Queue implementation
 */

#ifndef QUEUE_GROWTH_NUMBITS
  #define QUEUE_GROWTH_NUMBITS (4)
#endif
#define QUEUE_GROWTH_INCREMENT (1 << QUEUE_GROWTH_NUMBITS)
#define QUEUE_GROWTH_MASK (QUEUE_GROWTH_INCREMENT - 1)

QUEUE_TYPENAME* CreateQueue() {
  return calloc(1, sizeof(QUEUE_TYPENAME));
}

void ReleaseQueue( QUEUE_TYPENAME** queuePtr ) {
}

unsigned ReleaseQueueSlots( QUEUE_TYPENAME* queue,
  unsigned (*releaseSlot)(QUEUESLOT_TYPENAME* slotPtr) ) {
  return 3;
}

unsigned GrowQueue( QUEUE_TYPENAME* queue ) {
  return 2;
}

unsigned CompactQueue( QUEUE_TYPENAME* queue ) {
  return 2;
}

unsigned PushNext( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME fromItem ) {
  return 3;
}

unsigned PushLast( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME fromItem ) {
  return 3;
}

unsigned PopNext( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem ) {
  return 3;
}

unsigned PopLast( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem ) {
  return 3;
}

unsigned PeekNext( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem ) {
  return 3;
}

unsigned PeekNextAhead( QUEUE_TYPENAME* queue, unsigned byAmount,
    QUEUESLOT_TYPENAME* toItem ) {
  return 4;
}

unsigned PeekLast( QUEUE_TYPENAME* queue, QUEUESLOT_TYPENAME* toItem ) {
  return 3;
}

unsigned PeekLastFrom( QUEUE_TYPENAME* queue, unsigned amount,
    QUEUESLOT_TYPENAME* toItem ) {
  return 4;
}
