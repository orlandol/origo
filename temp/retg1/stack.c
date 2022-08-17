
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#ifndef STACK_GROWTH_SHIFT
  #define STACK_GROWTH_SHIFT 4
#endif

#define STACK_GROWTH_INCREMENT (1 << (STACK_GROWTH_SHIFT))
#define STACK_GROWTH_MASK (STACK_GROWTH_INCREMENT - 1)

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
  if( stack->bottom & STACK_GROWTH_MASK ) {
    // ...then the stack doesn't need to grow yet.
    return 0;
  }

  // ...else, grow by defined increment...
  newTop = (stack->top + STACK_GROWTH_INCREMENT) &
    (~STACK_GROWTH_MASK);
  newBottom = (stack->bottom + STACK_GROWTH_INCREMENT) &
    (~STACK_GROWTH_MASK);

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
  StackSlot* newSlots = NULL;
  unsigned newTop;
  unsigned newBottom;

printf( "CompactStack: Begin - stack(%p) slots(%p) top(%u) bottom(%u)\n",
stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
stack ? stack->bottom : 0 );

  if( stack == NULL ) { return 1; }

  // A NULL stack is compact
  if( stack->slot == NULL ) // { return 0; }
{ printf( "CompactStack: End 1\n\n" ); return 0; }

  newTop = stack->top - stack->bottom;
  newBottom = 0;
  if( stack->top ) {
printf( "CompactStack: if( stack->top ) - stack(%p) slots(%p) top(%u) bottom(%u) newTop(%u) newBottom(%u)\n",
stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
stack ? stack->bottom : 0, newTop, newBottom );

    if( newTop != newBottom ) {
      memmove( &stack->slot[0], &stack->slot[stack->bottom],
        newTop * sizeof(StackSlot) );

      newSlots = realloc(stack->slot,
        newTop * sizeof(StackSlot));
      if( newSlots == NULL ) { return 2; }

      stack->slot = newSlots;
      stack->top = newTop;
      stack->bottom = newBottom;
    } else {
      free( stack->slot );
      stack->slot = NULL;
      stack->top = 0;
      stack->bottom = 0;
    }

printf( "CompactStack: End 2 - stack(%p) slots(%p) top(%u) bottom(%u) newTop(%u) newBottom(%u)\n\n",
stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
stack ? stack->bottom : 0, newTop, newBottom );
    return 0;
  }

printf( "CompactStack: End 3 - stack(%p) slots(%p) top(%u) bottom(%u) newTop(%u) newBottom(%u)\n\n",
stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
stack ? stack->bottom : 0, newTop, newBottom );
  return 0;
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
  if( (stack == NULL) || (stack->top == stack->bottom) ) { return 1; }
  if( toItem == NULL ) { return 2; }

  (*toItem) = stack->slot[stack->bottom];

  return 0;
}

unsigned PeekAhead( Stack* stack, unsigned byAmount, StackSlot* toItem ) {
  if( stack == NULL ) { return 1; }

  if( byAmount > (stack->top - stack->bottom) ) { return 2; }

  if( toItem == NULL ) { return 3; }

  (*toItem) = stack->slot[stack->bottom + byAmount];

  return 0;
}
