
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#ifndef STACK_GROWTH_SHIFT
  #define STACK_GROWTH_SHIFT 4
#endif

#define STACK_GROWTH_INCREMENT (1 << (STACK_GROWTH_SHIFT))

Stack* CreateStack() {
  return calloc(1, sizeof(Stack));
}

void ReleaseStack( Stack** stackPtr ) {
  if( stackPtr ) {
    if( (*stackPtr) ) {
      if( (*stackPtr)->slot ) {
        free( (*stackPtr)->slot );
        (*stackPtr)->slot = NULL;
      }

      free( (*stackPtr) );
      (*stackPtr) = NULL;
    }
  }
}

unsigned GrowStack( Stack* stack ) {
  StackSlot* newSlots = NULL;
  unsigned newTop;
  unsigned newBottom;

  if( (stack == NULL) ||
    ((((unsigned)-1) - stack->top) < STACK_GROWTH_INCREMENT) ) {
    return 1;
  }

  // If any slots are left...
  if( stack->top & (STACK_GROWTH_INCREMENT - 1) ) {
    // ...then the stack doesn't need to grow yet.
    return 0;
  }

  // ...else, grow by defined increment...
  newBottom = (stack->bottom + STACK_GROWTH_INCREMENT) &
    (~(STACK_GROWTH_INCREMENT - 1));
  newTop = (stack->top + STACK_GROWTH_INCREMENT) &
    (~(STACK_GROWTH_INCREMENT - 1));

  // ...using realloc to reduce heap fragmentation...
  newSlots = realloc(stack->slot, newTop * sizeof(StackSlot));
  if( newSlots == NULL ) {
    return 2;
  }

  // ...at the potential cost of an extra memmove
  if( newTop != newBottom ) {
    memmove( &newSlots[newBottom], &newSlots[stack->bottom],
      (newTop - newBottom) * sizeof(StackSlot) );
    memset( &newSlots[stack->bottom], 0,
      STACK_GROWTH_INCREMENT * sizeof(StackSlot) );
  }

  stack->slot = newSlots;
  stack->bottom = newBottom;
  stack->top = newTop;

  return 0;
}

unsigned CompactStack( Stack* stack ) {
  return 2;
}

unsigned Push( Stack* stack, StackSlot fromItem ) {
  if( stack == NULL ) { return 1; }

  if( GrowStack(stack) ) {
    return 3;
  }

  if( stack->slot == NULL ) { return 4; }

  stack->bottom--;
  stack->slot[stack->bottom] = fromItem;

  return 0;
}

unsigned Pop( Stack* stack, StackSlot* toItem ) {
  if( (stack == NULL) || (stack->top == stack->bottom) ) { return 1; }
  if( toItem == NULL ) { return 2; }

  (*toItem) = stack->slot[stack->bottom];
  stack->bottom++;

  return 0;
}

unsigned Peek( Stack* stack, StackSlot* toItem ) {
  return 3;
}

unsigned PeekAhead( Stack* stack, unsigned byAmount, StackSlot* toItem ) {
  return 4;
}
