#include <stdlib.h>
#include <string.h>

#include "stack.h"

#ifndef STACK_GROWTH_NUMBITS
  #define STACK_GROWTH_NUMBITS (4)
#endif

#define STACK_GROWTH_INCREMENT (1 << STACK_GROWTH_NUMBITS)
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

void ReleasePointerStack( Stack** stackPtr,
  ReleaseSlotFunc releaseSlot ) {
  	
  StackSlot* slotPtr;
  unsigned index;

  if( stackPtr ) {
    if( (*stackPtr) ) {
      if( (*stackPtr)->slot ) {
        for( index = (*stackPtr)->bottom; index < (*stackPtr)->top; index++ ) {
          slotPtr = (*stackPtr)->slot[index];
      	
          if( releaseSlot(&slotPtr) ) {
            break;
          }
        }
    
        free( (*stackPtr)->slot );
        (*stackPtr)->slot = NULL;
      }
  
      free( (*stackPtr) );
      (*stackPtr) = NULL;
    }
  }
}

unsigned GrowStack( Stack* stack ) {
  StackSlot* newSlot = NULL;
  unsigned newTop;
  unsigned newBottom;

  if( stack == NULL ) { return 1; }

  if( (((unsigned)-1) - stack->top) < STACK_GROWTH_INCREMENT ) {
    return 2;
  }

  newTop = (stack->top & (~STACK_GROWTH_MASK)) + STACK_GROWTH_INCREMENT;
  newBottom = newTop - (stack->top - stack->bottom);

  if( stack->bottom == 0 ) {
    newSlot = realloc(stack->slot, newTop * sizeof(StackSlot));
    if( newSlot == NULL ) {
      return 3;
    }

    if( stack->slot ) {
      memmove( &newSlot[newBottom], &newSlot[stack->bottom],
        stack->bottom * sizeof(StackSlot) );
      memset( newSlot, 0, stack->bottom * sizeof(StackSlot) );
    }

    stack->slot = newSlot;
    stack->top = newTop;
    stack->bottom = newBottom;
  }

  return 0;
}

unsigned CompactStack( Stack* stack ) {
  StackSlot* newSlots = NULL;
  unsigned newTop;
  unsigned newBottom;

  if( (stack == NULL) || (stack->top < stack->bottom) ) { return 1; }

  newTop = stack->top - stack->bottom;
  newBottom = 0;

  if( newTop > newBottom ) {
    if( stack->slot ) {
      memmove( &stack->slot[0], &stack->slot[stack->bottom],
        stack->bottom * sizeof(StackSlot) );
    }

    newSlots = realloc(stack->slot, newTop * sizeof(StackSlot));
    if( newSlots == NULL ) {
      return 2;
    }

    stack->slot = newSlots;
    stack->top = newTop;
    stack->bottom = newBottom;

    return 0;
  }

  if( stack->slot ) {
    free( stack->slot );
    stack->slot = NULL;
  }

  stack->top = 0;
  stack->bottom = 0;

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

  if( byAmount >= (stack->top - stack->bottom) ) { return 2; }

  if( toItem == NULL ) { return 3; }

  (*toItem) = stack->slot[stack->bottom + byAmount];

  return 0;
}
