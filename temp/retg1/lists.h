#ifndef LISTS_H
#define LISTS_H

#include <stdlib.h>
#include <string.h>

///TODO: Re-write reserve boundary checks in GrowXYZ to reach unsigned's max
///TODO: Re-write boundary checks in GrowXYZ to succeed if reserve > 1
///TODO: Add functions to read a slot/item/etc value
///TODO: Merge Key Array into list as MAPARRAYxyz
///TODO: Change PushSlot parameter fromItem to a pointer
///TODO: Add a PushValue function to accept fromValue as a constant value

/*
 *  Stack declarations
 */

#define DECLARE_STACK_TYPES( STACKSLOT_TYPE, STACKSLOT_TYPENAME, STACK_TYPENAME )\
  \
  typedef STACKSLOT_TYPE STACKSLOT_TYPENAME;\
  \
  typedef struct STACK_TYPENAME {\
    STACKSLOT_TYPENAME* slot;\
    unsigned top;\
    unsigned bottom;\
  } STACK_TYPENAME;

#define DECLARE_DEFAULT_STACK_TYPES( STACKSLOT_TYPE )\
  DECLARE_STACK_TYPES( STACKSLOT_TYPE, StackSlot, Stack )

#define DECLARE_CREATESTACK( STACK_TYPENAME, FUNCNAME )\
STACK_TYPENAME* FUNCNAME();

#define DECLARE_RELEASESTACK( STACK_TYPENAME, FUNCNAME )\
void FUNCNAME( STACK_TYPENAME** stackPtr );

#define DECLARE_RELEASESTACKSLOTS( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack,\
  unsigned (*releaseSlot)(STACKSLOT_TYPENAME* slotPtr) );

#define DECLARE_GROWSTACK( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack );

#define DECLARE_COMPACTSTACK( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack );

#define DECLARE_PUSHSLOT( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME fromItem );

#define DECLARE_POPSLOT( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem );

#define DECLARE_PEEKSLOT( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem );

#define DECLARE_PEEKSLOTAHEAD( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, unsigned byAmount,\
  STACKSLOT_TYPENAME* toItem );

#define DECLARE_DEFAULT_STACK_FUNCTIONS() \
  DECLARE_CREATESTACK( Stack, CreateStack )\
  DECLARE_RELEASESTACK( Stack, ReleaseStack )\
  DECLARE_RELEASESTACKSLOTS( StackSlot, Stack, ReleaseStackSlots )\
  DECLARE_GROWSTACK( StackSlot, Stack, GrowStack )\
  DECLARE_COMPACTSTACK( StackSlot, Stack, CompactStack )\
  DECLARE_PUSHSLOT( StackSlot, Stack, Push )\
  DECLARE_POPSLOT( StackSlot, Stack, Pop )\
  DECLARE_PEEKSLOT( StackSlot, Stack, Peek )\
  DECLARE_PEEKSLOTAHEAD( StackSlot, Stack, PeekAhead )

/*
 *  Stack implementation
 */

#ifndef STACK_GROWTH_NUMBITS
  #define STACK_GROWTH_NUMBITS (4)
#endif
#define STACK_GROWTH_INCREMENT (1 << STACK_GROWTH_NUMBITS)
#define STACK_GROWTH_MASK (STACK_GROWTH_INCREMENT - 1)

#define IMPLEMENT_CREATESTACK( STACK_TYPENAME, FUNCNAME )\
STACK_TYPENAME* FUNCNAME() {\
  return calloc(1, sizeof(STACK_TYPENAME));\
}

#define IMPLEMENT_RELEASESTACK( STACK_TYPENAME, FUNCNAME )\
void FUNCNAME( STACK_TYPENAME** stackPtr ) {\
  if( stackPtr ) {\
    if( (*stackPtr) ) {\
      if( (*stackPtr)->slot ) {\
        free( (*stackPtr)->slot );\
        (*stackPtr)->slot = NULL;\
      }\
      \
      free( (*stackPtr) );\
        (*stackPtr) = NULL;\
    }\
  }\
}

#define IMPLEMENT_RELEASESTACKSLOTS( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack,\
  unsigned (*releaseSlot)(STACKSLOT_TYPENAME* slotPtr) ) {\
  \
  unsigned index;\
  \
  if( stack == NULL ) { return 1; }\
  if( releaseSlot == NULL ) { return 2; }\
  \
  if( stack->slot ) {\
    for( index = stack->bottom; index < stack->top; index++ ) {\
      if( releaseSlot(&stack->slot[index]) ) {\
        return 3;\
      }\
    }\
    \
    free( stack->slot );\
    stack->slot = NULL;\
  }\
  \
  return 0;\
}

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

#define IMPLEMENT_PUSHSLOT( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME fromItem ) {\
  if( stack == NULL ) { return 1; }\
  \
  if( GrowStack(stack) ) {\
    return 3;\
  }\
  \
  if( stack->slot == NULL ) { return 4; }\
  \
  stack->bottom--;\
  stack->slot[stack->bottom] = fromItem;\
  \
  return 0;\
}

#define IMPLEMENT_POPSLOT( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem ) {\
  if( (stack == NULL) || (stack->top == stack->bottom) ) { return 1; }\
  if( toItem == NULL ) { return 2; }\
  \
  (*toItem) = stack->slot[stack->bottom];\
  stack->bottom++;\
  \
  return 0;\
}

#define IMPLEMENT_PEEKSLOT( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem ) {\
  if( (stack == NULL) || (stack->top == stack->bottom) ) { return 1; }\
  if( toItem == NULL ) { return 2; }\
  \
  (*toItem) = stack->slot[stack->bottom];\
  \
  return 0;\
}

#define IMPLEMENT_PEEKSLOTAHEAD( STACKSLOT_TYPENAME, STACK_TYPENAME, FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, unsigned byAmount, STACKSLOT_TYPENAME* toItem ) {\
  if( stack == NULL ) { return 1; }\
  \
  if( byAmount >= (stack->top - stack->bottom) ) { return 2; }\
  \
  if( toItem == NULL ) { return 3; }\
  \
  (*toItem) = stack->slot[stack->bottom + byAmount];\
  \
  return 0;\
}

#define IMPLEMENT_STACK_DEFAULT_FUNCTIONS() \
  IMPLEMENT_CREATESTACK( Stack, CreateStack )\
  IMPLEMENT_RELEASESTACK( Stack, ReleaseStack )\
  IMPLEMENT_RELEASESTACKSLOTS( StackSlot, Stack, ReleaseStackSlots )\
  IMPLEMENT_GROWSTACK( StackSlot, Stack, GrowStack )\
  IMPLEMENT_COMPACTSTACK( StackSlot, Stack, CompactStack )\
  IMPLEMENT_PUSHSLOT( StackSlot, Stack, Push )\
  IMPLEMENT_POPSLOT( StackSlot, Stack, Pop )\
  IMPLEMENT_PEEKSLOT( StackSlot, Stack, Peek )\
  IMPLEMENT_PEEKSLOTAHEAD( StackSlot, Stack, PeekAhead )

#endif
